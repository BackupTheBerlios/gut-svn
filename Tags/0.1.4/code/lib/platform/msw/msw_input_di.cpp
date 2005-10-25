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
 * functionality in, say, DX5? I'd like to keep the system requirements as
 * low as possible */
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>


/* I configure the DirectInput devices to store events into a per-device
 * queue. This is the size of the queue per device. */
static const int MY_BUFFER_SIZE = 32;
static const int BAD_INDEX = -1;

/* I keep one of these for each attached input device */
struct MyDeviceDesc
{
	int kind;
	int index;
	IDirectInputDevice8* device;
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

static IDirectInput8* my_di = NULL;
static utxArray<MyDeviceDesc*> my_devices;

int utx_msw_InitializeInput()
{
	HRESULT hr;

	hr = DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&my_di, NULL);
	if (FAILED(hr))
	{
		utx_msw_ReportError("DirectInput8Create", hr);
		return false;
	}

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

static void myAcquireDevice(IDirectInputDevice8* device);
static void mySendKeyboardEvent(MyDeviceDesc* desc, DWORD currentTime);

int utx_msw_PollInputDevices()
{
	if (utGetActiveWindow() == NULL)
		return true;

	for (int ixDevice = 0; ixDevice < my_devices.size(); ++ixDevice)
	{
		MyDeviceDesc* desc = my_devices[ixDevice];
		IDirectInputDevice8* device = desc->device;

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
		hr = device->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), desc->buffer, &desc->numEvents, NULL);
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
				default:
					nextDeviceToSend->nextEvent++;  /* temporary, until I port handlers */
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

#if NOT_READY_YET
void mySendMouseEvent(const MyDeviceDesc* desc, const DIDEVICEOBJECTDATA& data)
{
	int what, index, value;
	switch (data.dwOfs)
	{
	case DIMOFS_BUTTON0:
	case DIMOFS_BUTTON1:
	case DIMOFS_BUTTON2:
	case DIMOFS_BUTTON3:
		what = UT_EVENT_MOUSE_BUTTON;
		index = data.dwOfs - DIMOFS_BUTTON0;
		value = (data.dwData & 0x80) ? 1 : 0;
		break;
	case DIMOFS_X:
		what = UT_EVENT_MOUSE_AXIS;
		index = 0;
		value = data.dwData;
		break;
	case DIMOFS_Y:
		what = UT_EVENT_MOUSE_AXIS;
		index = 1;
		value = data.dwData;
		break;
	case DIMOFS_Z:
		what = UT_EVENT_MOUSE_AXIS;
		index = 2;
		value = data.dwData;
		break;
	}

	int mouse = desc->index;
	DWORD when = data.dwTimeStamp;

	utxSendInputEvent(what, window, when, mouse, index, value);
}
#endif


/****************************************************************************
 * Helper function to convert a DirectInput controller event into a Toolkit
 * event and send it up to the host.
 ****************************************************************************/

#if NOT_READY_YET
void mySendControllerEvent(utWindow window, const MyDeviceDesc* device, const DIDEVICEOBJECTDATA& data)
{
	int what, index, value;
	switch (data.dwOfs)
	{
	case DIJOFS_X:
	case DIJOFS_Y:
	case DIJOFS_Z:
	case DIJOFS_RX:
	case DIJOFS_RY:
	case DIJOFS_RZ:
	case DIJOFS_SLIDER(0):
	case DIJOFS_SLIDER(1):
		what  = UT_EVENT_CTRL_AXIS;
		index = (data.dwOfs - DIJOFS_X) / sizeof(LONG);
		value = data.dwData;
		break;
	default:
		what = UT_EVENT_CTRL_BUTTON;
		index = (data.dwOfs - DIJOFS_BUTTON(0)) / sizeof(BYTE);
		value = (data.dwData & 0x80) ? 1 : 0;
		break;
	}

	int controller = device->state->index;
	DWORD when = data.dwTimeStamp;
	
	utxSendInputEvent(what, window, when, controller, index, value);
}
#endif


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
	IDirectInputDevice8* idevice;
	HRESULT hr = my_di->CreateDevice(deviceInfo->guidInstance, &idevice, NULL);
	if (FAILED(hr)) 
	{
		utx_msw_ReportError("IDirectInput8::CreateDevice", hr);
		return DIENUM_CONTINUE;
	}

	/* Set the data format for event buffering */
	int kind, index;
	switch (deviceInfo->dwDevType & 0xff)
	{
	case DI8DEVTYPE_KEYBOARD:
		hr = idevice->SetDataFormat((LPCDIDATAFORMAT)&c_dfDIKeyboard);
		kind = UT_DEVICE_KEYBOARD;
		index = utNumKeyboards();
		break;

	case DI8DEVTYPE_MOUSE:
		hr = idevice->SetDataFormat((LPCDIDATAFORMAT)&c_dfDIMouse);
		kind = UT_DEVICE_MOUSE;
		index = utNumMice();
		break;

	case DI8DEVTYPE_DRIVING:
	case DI8DEVTYPE_GAMEPAD:
	case DI8DEVTYPE_JOYSTICK:
		hr = idevice->SetDataFormat((LPCDIDATAFORMAT)&c_dfDIJoystick);
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

void myAcquireDevice(IDirectInputDevice8* device)
{
	HRESULT hr;

	device->Unacquire();

	HWND hwnd = (HWND)utGetWindowHandle(utGetActiveWindow());
	hr = device->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NOWINKEY | DISCL_NONEXCLUSIVE);
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
