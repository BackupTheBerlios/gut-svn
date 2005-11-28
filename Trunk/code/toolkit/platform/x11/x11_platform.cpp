/**********************************************************************
 * GameGut - x11_platform.cpp
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
#include "x11_platform.h"

Display* utx_display = NULL;
Atom     utx_wmDeleteAtom;

int utxInitializePlatform()
{
	/* I create a single connection to the display and share it among all of
	 * the windows. This might cause trouble with multihead systems */
	utx_display = XOpenDisplay(NULL);
	if (utx_display == NULL)
		return utxReportX11Error("XOpenDisplay");

	/* Setup "window closed" notifications */
	utx_wmDeleteAtom = XInternAtom(utx_display, "WM_DELETE_WINDOW", False);

	/* I just assume that a keyboard and mouse are attached */
	utLog("Found keyboard\n");
	utxRegisterInputDevice(UT_DEVICE_KEYBOARD, 255);
	utLog("Found mouse\n");
	utxRegisterInputDevice(UT_DEVICE_MOUSE, 3);

	return true;
}


int utxShutdownPlatform()
{
	if (!utxDestroyAllWindows())
		return false;

	/* Free memory used to keep track of user input devices */
	utxReleaseAllInputDevices();
		
	XCloseDisplay(utx_display);
	utx_display = NULL;
	return true;
}
