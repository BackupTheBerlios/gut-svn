/**********************************************************************
 * GameGut - x11_window.cpp
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

utWindow utxCreateWindow(const char* title, int width, int height)
{
	int screen = DefaultScreen(utx_display);

	/* Create the window */
	XSetWindowAttributes attributes;
	attributes.border_pixel = 0;
	attributes.colormap = XDefaultColormap(utx_display, screen);
	attributes.event_mask = StructureNotifyMask | ExposureMask |
	                        VisibilityChangeMask | FocusChangeMask |
	                        ButtonPressMask | ButtonReleaseMask |
	                        PointerMotionMask | ButtonMotionMask |
	                        KeyPressMask | KeyReleaseMask |
	                        EnterWindowMask | LeaveWindowMask;

	Window window = XCreateWindow(utx_display, RootWindow(utx_display, screen),
	                       32, 32, width, height, 0,
	                       CopyFromParent, InputOutput, CopyFromParent,
	                       CWBorderPixel | CWColormap | CWEventMask, 
	                       &attributes);
	if (!window)
	{
		utxReportX11Error("XCreateWindow");
		return NULL;
	}

	/* Set the window caption */
	int   argc = 0;
	char* argv = "\0";
	XSetStandardProperties(utx_display, window, title, title, 0, &argv, argc, NULL);

	/* Show the window */
	XSetWMProtocols(utx_display, window, &utx_wmDeleteAtom, 1);
	XMapWindow(utx_display, window);

	utWindow result = utALLOCT(utxWindow);
	result->screen  = screen;
	result->handle  = (void*)window;
	result->window  = window;
	return result;
}


int utxDestroyWindow(utWindow window)
{
	if (window != NULL)
	{
		XDestroyWindow(utx_display, window->window);
		utFREE(window);
	}
	return true;
}
