/**********************************************************************
 * GameGut - ut_window.cpp
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

static int mySetActiveWindow(utWindow window);

/* Keep track of all open windows so I can close them at exit */
static utxArray<utWindow> my_windows;


utWindow utCreateWindow(const char* title, int width, int height)
{
	/* utxCreateWindow() is a platform-specific create function */
	utWindow window = utxCreateWindow(title, width, height);
	if (window != NULL)
	{
		my_windows.push_back(window);
		mySetActiveWindow(window);
	}
	return window;
}


int utDestroyWindow(utWindow window)
{
	/* utxDestroyWindow() is a platform-specific destroy function */
	my_windows.remove(window);
	return utxDestroyWindow(window);
}


utWindow utFindWindowByHandle(void* handle)
{
	for (int i = 0; i < my_windows.size(); ++i)
	{
		if (((utxWindowBase*)my_windows[i])->handle == handle)
			return (my_windows[i]);
	}
	return NULL;
}


void* utGetWindowHandle(utWindow window)
{
	return ((utxWindowBase*)window)->handle;
}


int utNumWindows()
{
	return my_windows.size();
}


/****************************************************************************
 * Called by the platform-specific utxShutdownPlatform(); closes all 
 * remaining windows and release all memory associated with the list.
 ****************************************************************************/

int utxDestroyAllWindows()
{
	for (int i = 0; i < my_windows.size(); ++i)
		utxDestroyWindow(my_windows[i]);
	my_windows.clear();
	return true;
}


/****************************************************************************
 * Called in window creation and focus change events. Notifies the rest of
 * the system that a new window (or no window) has the focus.
 ****************************************************************************/

static utWindow my_activeWindow = NULL;

utWindow utGetActiveWindow()
{
	return my_activeWindow;
}

int mySetActiveWindow(utWindow window)
{
	my_activeWindow = window;
	utxInputFocusChanged(window);
	return true;
}


/****************************************************************************
 * Called by the platform-specific window systems instead of utSendEvent();
 * handles cross-platform responses to important messages.
 ****************************************************************************/

int utxSendWindowEvent(utEvent* event)
{
	switch (event->what)
	{
	case UT_EVENT_WINDOW_FOCUS:
		mySetActiveWindow(event->arg0 != 0 ? event->window : NULL);
		break;
	}

	return utSendEvent(event);
}

