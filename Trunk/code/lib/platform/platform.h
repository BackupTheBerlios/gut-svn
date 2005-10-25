/**********************************************************************
 * GameGut - platform.h
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

int utxInitializePlatform();
int utxShutdownPlatform();


enum utxInputDeviceKind
{
	UT_DEVICE_NONE,
	UT_DEVICE_KEYBOARD,
	UT_DEVICE_MOUSE,
	UT_DEVICE_CONTROLLER
};


/* Maximum range for input object states */
#define MAX_INPUT   (1 << 30)

/* A base class for platform-specific window objects. Exposing the
 * handle allows me to locate windows in a platform-generic way */
struct utxWindowBase
{
	void* handle;
};


/* Platform-specific functions */
utWindow utxCreateWindow(const char* title, int width, int height);
int      utxDestroyWindow(utWindow window);
int      utxDestroyAllWindows();
int      utxPollEvents(int block);
int      utxSendWindowEvent(utEvent* event);


/* User input device management */
int      utxRegisterInputDevice(int type, int numBtns);
int      utxReleaseAllInputDevices();
int      utxInputFocusChanged(utWindow window);
int      utxResetInputPlatform();
int      utxSendInputEvent(utEvent* event);
