/**********************************************************************
 * GameGut - msw_input_di.cpp
 * Copyright (c) 1999-2005 Jason Perkins.
 * All rights reserved.
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the BSD-style license that is 
 * included with this library in the file LICENSE.txt.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
 * files LICENSE.txt for more details. 
 **********************************************************************/

#include "core/core.h"
#include "msw_platform.h"
#include <stdio.h>


/* I'm using DX8 because that was the current version when I last overhauled
 * this code. Do I really need this version, or could I get all of the same
 * functionality in, say, DX5? (And does it matter?) */
// #define DIRECTINPUT_VERSION 0x0800
#define DIRECTINPUT_VERSION 0x0500
#include <dinput.h>

/* I configure the DirectInput devices to store events into a per-device
 * queue. This is the size of the queue per device. */
static const int MY_BUFFER_SIZE = 32;
static const DWORD BAD_INDEX = 0xffffffff;

/* I keep one of these for each attached input device */
struct MyDeviceDesc
{
	int kind;
	int index;
//	IDirectInputDevice8* device;
	IDirectInputDevice2* device;
	DIDEVICEOBJECTDATA buffer[MY_BUFFER_SIZE];
	DWORD numEvents;
	DWORD nextEvent;
	
	int   keyRepeatEvent;
	DWORD keyRepeatTime;
};

/* DirectInput callback for each device found */
BOOL CALLBACK utx_msw_EnumDevicesCallback(LPCDIDEVICEINSTANCE deviceInfo, LPVOID data);

/* DirectInput doesn't support key repeat, so I simulate it */
static DWORD my_initialDelay;
static DWORD my_repeatDelay;

/* DirectInput doesn't support character output, so I simulate that too */
static HKL my_keyboardLayout;


/****************************************************************************
 * Called during platform initialization; connect to DirectInput and scan
 * for any attached user input devices. Devices are identified and stuck 
 * into a master list locally, as well as registered with the main input
 * system in ut_input.cpp.
 ****************************************************************************/

// static IDirectInput8* my_di = NULL;
static IDirectInput* my_di = NULL;
static utxArray<MyDeviceDesc*> my_devices;

int utx_msw_InitializeInput()
{
	HRESULT hr;

	/* Connect to DirectInput */
//	hr = DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&my_di, NULL);
	hr = DirectInputCreate(GetModuleHandle(NULL), DIRECTINPUT_VERSION, &my_di, NULL);
	if (FAILED(hr))
	{
		utx_msw_ReportError("DirectInput8Create", hr);
		return false;
	}

	/* Enumerate all attached devices */
	hr = my_di->EnumDevices(0, utx_msw_EnumDevicesCallback, NULL, DIEDFL_ATTACHEDONLY);
	if (FAILED(hr))
	{
		utLog("Failed to enumerate input devices\n");
		utx_msw_ShutdownInput();
		return false;
	}

	/* Grab the key repeat settings, I handle autorepeat here */
	int value;
	SystemParametersInfo(SPI_GETKEYBOARDDELAY, 0, &value, 0);
	my_initialDelay = 250 * value + 250;
	SystemParametersInfo(SPI_GETKEYBOARDSPEED, 0, &value, 0);
	my_repeatDelay = 400 - (12 * value);

	/* Grab the keyboard layout, for conversions to character output */
	my_keyboardLayout = GetKeyboardLayout(0);

	return true;
}


/****************************************************************************
 * Called during platform shutdown; disconnect from DirectInput and release
 * all of my device interface objects.
 ****************************************************************************/

int utx_msw_ShutdownInput()
{
	if (my_di != NULL)
	{
		for (int i = 0; i < my_devices.size(); ++i)
		{
			my_devices[i]->device->Release();
			utFREE(my_devices[i]);
		}
		my_devices.clear();

		my_di->Release();
		my_di = NULL;
	}

	utxReleaseAllInputDevices();
	return true;
}


/****************************************************************************
 * Called by utPollEvents() (ut_events.cpp) by way of utxPollEvents() 
 * (msw_events.cpp). Polls all of the registered devices and then sends off
 * any queued events. Some extra work is necessary here to make sure that
 * the events all get shipped off in chronological order.
 ****************************************************************************/

// static void myAcquireDevice(IDirectInputDevice8* device);
static void myAcquireDevice(IDirectInputDevice2* device);
static void mySendKeyboardEvent(MyDeviceDesc* desc, DWORD currentTime);
static void mySendMouseEvent(MyDeviceDesc* desc, const DIDEVICEOBJECTDATA& data);
static void mySendCtrlEvent(MyDeviceDesc* desc, const DIDEVICEOBJECTDATA& data);

int utx_msw_PollInputDevices()
{
	if (utGetActiveWindow() == NULL)
		return true;

	for (int ixDevice = 0; ixDevice < my_devices.size(); ++ixDevice)
	{
		MyDeviceDesc* desc = my_devices[ixDevice];
//		IDirectInputDevice8* device = desc->device;
		IDirectInputDevice2* device = desc->device;

		/* Poll the device. If this fails, reacquire and try again next time */
		HRESULT hr = device->Poll();
		if (FAILED(hr))
		{
			myAcquireDevice(device);
			continue;
		}

		/* Copy the data out of the device */
		desc->nextEvent = 0;
		desc->numEvents = MY_BUFFER_SIZE;
		hr = device->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), desc->buffer, &(desc->numEvents), 0);
		if (FAILED(hr))
		{
			utx_msw_ReportError("IDirectInputDevice8::GetDeviceData", hr);
			desc->numEvents = 0;
			continue;
		}
	}

	/* Send off the events, in chronological order */
	DWORD currentTime = utGetTimer();
	MyDeviceDesc* nextDeviceToSend;
	do
	{
		nextDeviceToSend = NULL;

		/* Find earliest event, brute-force style */
		DWORD lowest = (DWORD)(-1);
		for (int ix = 0; ix < my_devices.size(); ++ix)
		{
			MyDeviceDesc* desc = my_devices[ix];
			if (desc->nextEvent < desc->numEvents)
			{
				DWORD timestamp = desc->buffer[desc->nextEvent].dwTimeStamp;
				if (timestamp < lowest)
				{
					lowest = timestamp;
					nextDeviceToSend = desc;
				}
			}

			/* Check for button autorepeats */
			if (desc->numEvents == 0 && desc->keyRepeatEvent != BAD_INDEX && desc->keyRepeatTime <= currentTime)
			{
				if (desc->keyRepeatTime < lowest)
				{
					lowest = desc->keyRepeatTime;
					nextDeviceToSend = desc;
				}
			}

			/* Send it off */
			if (nextDeviceToSend != NULL)
			{
				switch (nextDeviceToSend->kind)
				{
				case UT_DEVICE_KEYBOARD:
					mySendKeyboardEvent(nextDeviceToSend, currentTime);
					break;
				case UT_DEVICE_MOUSE:
					mySendMouseEvent(nextDeviceToSend, nextDeviceToSend->buffer[nextDeviceToSend->nextEvent++]);
					break;
				case UT_DEVICE_CONTROLLER:
					mySendCtrlEvent(nextDeviceToSend, nextDeviceToSend->buffer[nextDeviceToSend->nextEvent++]);
					break;
				default:
					nextDeviceToSend->nextEvent++;
					break;
				}
			}
		}

	} while (nextDeviceToSend != NULL);

	return true;
}


/****************************************************************************
 * Called by utxInputFocusChanged() in ut_input.cpp when the user has
 * switched to a different application. Shut down any active autorepeats.
 ****************************************************************************/

int utxResetInputPlatform()
{
	for (int i = 0; i < my_devices.size(); ++i)
		my_devices[i]->keyRepeatEvent = BAD_INDEX;
	return true;
}


/****************************************************************************
 * Helper function to convert a DirectInput keyboard event into a Toolkit
 * event and send it up to the host.
 ****************************************************************************/

extern int utx_msw_keymap[];   /* defined in msw_input_keymap.cpp */

void mySendKeyboardEvent(MyDeviceDesc* desc, DWORD currentTime)
{
	utEvent event;
	event.window = utGetActiveWindow();
	event.arg0   = desc->index;

	const DIDEVICEOBJECTDATA* data;
	if (desc->nextEvent < desc->numEvents)
	{
		/* Process a normal key press/release */
		data = &(desc->buffer[desc->nextEvent]);
		event.what = UT_EVENT_KEY;
		event.when = data->dwTimeStamp;
		event.arg1 = utx_msw_keymap[data->dwOfs];
		event.arg2 = (data->dwData & 0x80) ? MAX_INPUT : 0;

		desc->keyRepeatEvent = (event.arg2 != 0) ? desc->nextEvent : BAD_INDEX;
		desc->keyRepeatTime  = currentTime + my_initialDelay;
		desc->nextEvent++;
	}
	else if (desc->keyRepeatEvent != BAD_INDEX)
	{
		/* Process an autorepeat */
		data = &(desc->buffer[desc->keyRepeatEvent]);
		event.what = UT_EVENT_KEY_REPEAT;
		event.when = desc->keyRepeatTime;
		event.arg1 = utx_msw_keymap[data->dwOfs];
		event.arg2 = MAX_INPUT;

		desc->keyRepeatTime += my_repeatDelay;
	}
	else
	{
		/* Why am I here? */
		return;
	}

	utxSendInputEvent(&event);

	/* If this is a key press or repeat, also send a char */
	if (event.arg2 != 0)
	{
		BYTE keys[256];
		GetKeyboardState(keys);

		UINT key = MapVirtualKeyEx(data->dwOfs, 1, my_keyboardLayout);

		WORD ch;
		int c = ToAsciiEx(key, data->dwOfs, keys, &ch, 0, my_keyboardLayout);
		if (c > 0)
		{
			event.what = UT_EVENT_CHAR;
			event.arg1 = (c == 1) ? ch & 0xff : ch;
			utxSendInputEvent(&event);
		}
	}
}


/****************************************************************************
 * Helper function to convert a DirectInput mouse event into a Toolkit
 * event and send it up to the host.
 ****************************************************************************/

void mySendMouseEvent(MyDeviceDesc* desc, const DIDEVICEOBJECTDATA& data)
{
	utEvent event;
	event.when   = data.dwTimeStamp;
	event.window = utGetActiveWindow();
	event.arg0   = desc->index;

	/* GCC doesn't consider DIMOFS_... values constants, so can't use switch */
	if (data.dwOfs == DIMOFS_BUTTON0 ||
	    data.dwOfs == DIMOFS_BUTTON1 ||
		 data.dwOfs == DIMOFS_BUTTON2 ||
		 data.dwOfs == DIMOFS_BUTTON3)
	{
		event.what = UT_EVENT_MOUSE_BUTTON;
		event.arg1 = data.dwOfs - DIMOFS_BUTTON0;
		event.arg2 = (data.dwData & 0x80) ? MAX_INPUT : 0;
	}
	else if (data.dwOfs == DIMOFS_X)
	{
		event.what = UT_EVENT_MOUSE_AXIS;
		event.arg1 = 0;
		event.arg2 = data.dwData;
	}
	else if (data.dwOfs == DIMOFS_Y)
	{
		event.what = UT_EVENT_MOUSE_AXIS;
		event.arg1 = 1;
		event.arg2 = data.dwData;
	}
	else if (data.dwOfs == DIMOFS_Z)
	{
		event.what = UT_EVENT_MOUSE_AXIS;
		event.arg1 = 2;
		event.arg2 = data.dwData;
	}
	else
	{
		return;
	}

	utxSendInputEvent(&event);
}


/****************************************************************************
 * Helper function to convert a DirectInput controller event into a Toolkit
 * event and send it up to the host.
 ****************************************************************************/

void mySendCtrlEvent(MyDeviceDesc* desc, const DIDEVICEOBJECTDATA& data)
{
	utEvent event;
	event.when   = data.dwTimeStamp;
	event.window = utGetActiveWindow();
	event.arg0   = desc->index;

	/* GCC doesn't consider DIMOFS_... values constants, so can't use switch */
	if (data.dwOfs == DIJOFS_X ||
		 data.dwOfs == DIJOFS_Y ||
		 data.dwOfs == DIJOFS_Z ||
		 data.dwOfs == DIJOFS_RX ||
		 data.dwOfs == DIJOFS_RY ||
		 data.dwOfs == DIJOFS_RZ ||
		 data.dwOfs == DIJOFS_SLIDER(0) ||
		 data.dwOfs == DIJOFS_SLIDER(1))
	{
		event.what = UT_EVENT_CTRL_AXIS;
		event.arg1 = (data.dwOfs - DIJOFS_X) / sizeof(LONG);
		event.arg2 = data.dwData;
	}
	else
	{
		event.what = UT_EVENT_CTRL_BUTTON;
		event.arg1 = (data.dwOfs - DIJOFS_BUTTON(0)) / sizeof(BYTE);
		event.arg2 = (data.dwData & 0x80) ? MAX_INPUT : 0;
	}

	utxSendInputEvent(&event);
}


/****************************************************************************
 * Called by DirectInput as part of device enumeration, which is kicked off
 * in utx_msw_InitializeInput(). Identifies the device, then creates my own
 * interface to it, stores it in a local list, and also registers it with
 * the main input system in ut_input.cpp.
 ****************************************************************************/

BOOL CALLBACK utx_msw_EnumDevicesCallback(LPCDIDEVICEINSTANCE deviceInfo, LPVOID data)
{
	/* Write some information about the device to the log */
	char msg[512];
	sprintf(msg, "Found %s...", deviceInfo->tszInstanceName);
	utLog(msg);

	/* Open a connection to the device */
//	IDirectInputDevice8* idevice;
	IDirectInputDevice2* idevice;
//	HRESULT hr = my_di->CreateDevice(deviceInfo->guidInstance, &idevice, NULL);
	HRESULT hr = my_di->CreateDevice(deviceInfo->guidInstance, (IDirectInputDevice**)&idevice, NULL);
	if (FAILED(hr)) 
	{
		utx_msw_ReportError("IDirectInput::CreateDevice", hr);
		return DIENUM_CONTINUE;
	}

	/* Set the data format for event buffering */
	int kind, index;
	switch (deviceInfo->dwDevType & 0xff)
	{
//	case DI8DEVTYPE_KEYBOARD:
	case DIDEVTYPE_KEYBOARD:
		hr = idevice->SetDataFormat((LPCDIDATAFORMAT)&c_dfDIKeyboard);
		kind = UT_DEVICE_KEYBOARD;
		index = utNumKeyboards();
		break;

//	case DI8DEVTYPE_MOUSE:
	case DIDEVTYPE_MOUSE:
		hr = idevice->SetDataFormat((LPCDIDATAFORMAT)&c_dfDIMouse);
		kind = UT_DEVICE_MOUSE;
		index = utNumMice();
		break;

//	case DI8DEVTYPE_DRIVING:
//	case DI8DEVTYPE_GAMEPAD:
//	case DI8DEVTYPE_JOYSTICK:
	case DIDEVTYPE_JOYSTICK:
		hr = idevice->SetDataFormat((LPCDIDATAFORMAT)&c_dfDIJoystick2);
		kind = UT_DEVICE_CONTROLLER;
		index = utNumControllers();
		break;

	default:
		idevice->Release();
		utLog("unused\n");
		return DIENUM_CONTINUE;
	}

	if (FAILED(hr))
	{
		idevice->Release();
		utx_msw_ReportError("IDirectInputDevice8::SetDataFormat", hr);
		return DIENUM_CONTINUE;
	}

	/* Allocate a buffer to hold device events */
	DIPROPDWORD buffer;
	buffer.diph.dwSize       = sizeof(DIPROPDWORD);
	buffer.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	buffer.diph.dwHow        = DIPH_DEVICE;
	buffer.diph.dwObj        = 0;
	buffer.dwData            = MY_BUFFER_SIZE;
	hr = idevice->SetProperty(DIPROP_BUFFERSIZE, (DIPROPHEADER*)&buffer);
	if (FAILED(hr))
	{
		idevice->Release();
		utx_msw_ReportError("IDirectInputDevice8::SetProperty", hr);
		return DIENUM_CONTINUE;
	}

	/* Retreive the number of whatsits and doodads on the device */
	DIDEVCAPS devCaps;
	ZeroMemory(&devCaps, sizeof(DIDEVCAPS));
	devCaps.dwSize = sizeof(DIDEVCAPS);
	hr = idevice->GetCapabilities(&devCaps);
	if (FAILED(hr)) 
	{
		idevice->Release();
		utx_msw_ReportError("IDirectInputDevice8::GetCapabilities", hr);
		return DIENUM_CONTINUE;
	}

	/* Add this device to my master list */
	MyDeviceDesc* desc = utALLOCT(MyDeviceDesc);
	desc->device = idevice;
	desc->kind   = kind;
	desc->index  = index;
	desc->keyRepeatEvent = BAD_INDEX;
	my_devices.push_back(desc);

	/* Register this device with the input system */
	utxRegisterInputDevice(kind, devCaps.dwButtons);

	utLog("ok\n");
	return DIENUM_CONTINUE;
}


/****************************************************************************
 * Called by utx_msw_PollInputDevices() (above) when it is unable to poll
 * a device. Tries to reacquire the device for the next pass.
 ****************************************************************************/

//void myAcquireDevice(IDirectInputDevice8* device)
void myAcquireDevice(IDirectInputDevice2* device)
{
	HRESULT hr;

	device->Unacquire();

	HWND hwnd = (HWND)utGetWindowHandle(utGetActiveWindow());
//	hr = device->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NOWINKEY | DISCL_NONEXCLUSIVE);
	hr = device->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (SUCCEEDED(hr))
	{
		hr = device->Acquire();
		if (FAILED(hr))
			utx_msw_ReportError("IDirectInputDevice8::Acquire", hr);
	}
	else
	{
		utx_msw_ReportError("IDirectInputDevice8::SetCooperativeLevel", hr);
	}
}
