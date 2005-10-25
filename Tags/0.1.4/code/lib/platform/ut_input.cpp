/**********************************************************************
 * GameGut - ut_input.cpp
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
#include "platform/platform.h"


/* Keep track of the state of every object on every device */
struct MyInputDevice
{
	int  type;
	int* buttons;
	int  numButtons;
};

/* Lists of all input devices connected to the system */
static utxArray<MyInputDevice*> my_keyboards;
static utxArray<MyInputDevice*> my_mice;
static utxArray<MyInputDevice*> my_controllers;


/****************************************************************************
 * Functions to query device states
 ****************************************************************************/

int utNumControllers()
{
	return my_controllers.size();
}

int utNumKeyboards()
{
	return my_keyboards.size();
}

int utNumMice()
{
	return my_mice.size();
}


/****************************************************************************
 * Called by the platform-specific input system initialization; adds a new
 * device to the master lists that are managed here.
 ****************************************************************************/

int utxRegisterInputDevice(int type, int numBtns)
{
	MyInputDevice* device = utALLOCT(MyInputDevice);
	device->type = type;

	device->buttons = (int*)utALLOC(sizeof(int) * numBtns);
	device->numButtons = numBtns;
	memset(device->buttons, 0, sizeof(int) * numBtns);

	switch (type)
	{
	case UT_DEVICE_KEYBOARD:
		my_keyboards.push_back(device);
		break;
	case UT_DEVICE_MOUSE:
		my_mice.push_back(device);
		break;
	case UT_DEVICE_CONTROLLER:
		my_controllers.push_back(device);
		break;
	}

	return true;
}


/****************************************************************************
 * Called by the platform-specific input system during shutdown; release
 * all of the input device objects and the memory associated with them.
 ****************************************************************************/

void utxReleaseInputDevice(MyInputDevice* device)
{
	utFREE(device->buttons);
	utFREE(device);
}

int utxReleaseAllInputDevices()
{
	int i;

	for (i = 0; i < my_keyboards.size(); ++i)
		utxReleaseInputDevice(my_keyboards[i]);
	my_keyboards.clear();

	for (i = 0; i < my_mice.size(); ++i)
		utxReleaseInputDevice(my_mice[i]);
	my_mice.clear();

	for (i = 0; i < my_controllers.size(); ++i)
		utxReleaseInputDevice(my_controllers[i]);
	my_controllers.clear();

	return true;
}


/****************************************************************************
 * Called by mySetActiveWindow() in ut_window.cpp whenever the active 
 * window has changed. 
 ****************************************************************************/

static utWindow my_window = NULL;

static void myReleaseButtons(utxArray<MyInputDevice*>& devices, utEvent* event)
{
	for (int ixDev = 0; ixDev < devices.size(); ++ixDev)
	{
		event->arg0 = ixDev;

		int* buttons = devices[ixDev]->buttons;
		int  count   = devices[ixDev]->numButtons;

		for (int ixBtn = 0; ixBtn < count; ++ixBtn)
		{
			if (buttons[ixBtn])
			{
				event->arg1 = ixBtn;
				utxSendInputEvent(event);
			}
		}
	}
}

int utxInputFocusChanged(utWindow window)
{
	if (window == NULL)
	{
		utEvent event;
		event.window = my_window;
		event.when   = utGetTimer();
		event.arg2   = 0;

		/* Release any buttons that are marked as down, since I won't be
		 * notified if the user releases them while in the other app */
		event.what = UT_EVENT_KEY;
		myReleaseButtons(my_keyboards, &event);

		/* Let the platform layer clean up as well */
		utxResetInputPlatform();
	}

	my_window = window;
	return true;
}


/****************************************************************************
 * Called by the platform-specific input system rather than utSendEvent().
 * Updates the input device state stored here.
 ****************************************************************************/

int utxSendInputEvent(utEvent* event)
{
	int* state;
	switch (event->what)
	{
	case UT_EVENT_KEY:
		state = &(my_keyboards[event->arg0]->buttons[event->arg1]);
		if (*state == event->arg2)
			return 1;
		*state = event->arg2;
		break;
	}

	return utSendEvent(event);
}
