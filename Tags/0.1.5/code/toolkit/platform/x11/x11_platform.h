/**********************************************************************
 * GameGut - x11_platform.h
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

#include "platform/platform.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>

extern Display* utx_display;
extern Atom     utx_wmDeleteAtom;

struct utxWindow : utxWindowBase
{
	int      screen;
	Window   window;
	int      width, height;

	~utxWindow()
	{
		XDestroyWindow(utx_display, window);
	}
};

int utxReportX11Error_(const char* function, const char* file, int line);
#define utxReportX11Error(func)  utxReportX11Error_(func, __FILE__, __LINE__)
