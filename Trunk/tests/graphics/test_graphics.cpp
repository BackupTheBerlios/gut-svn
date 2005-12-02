/**********************************************************************
 * GameGut - test_graphics.cpp
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

#include <gut/gut.h>
#include <stdio.h>
#include <stdlib.h>
#if defined(_WIN32)
#include <windows.h>
#endif

static utRenderTarget target;
static bool keepRunning;


void UT_CALLBACK onEvent(utEvent* event)
{
	switch (event->what)
	{
	case UT_EVENT_KEY:
		if (event->arg1 == UT_KEY_ESCAPE && event->arg2 > 0)
			keepRunning = false;
		break;

	case UT_EVENT_WINDOW_CLOSE:
		utDestroyWindow(event->window);
		break;

	case UT_EVENT_WINDOW_RESIZE:
		utResizeRenderTarget(target, event->arg0, event->arg1);
		break;
	}
}


void UT_CALLBACK onLogMessage(const char* message)
{
	printf(message);
#if defined(_WIN32)
	OutputDebugString(message);
#endif
}


void die(const char* msg)
{
	puts(msg);
	exit(1);
}


void tick()
{
	utBeginFrame();
	utClear(0.2f, 0.0f, 0.2f, 1.0f);
	utEndFrame();
	utSwapRenderTarget(target);
}


int main()
{
	utSetLogHandler(onLogMessage);
	utInitialize();

	utWindow wnd = utCreateWindow("Toolkit Graphics Test", 640, 480);
	if (wnd == NULL)
		die("Failed to create window");

	void* hwnd = utGetWindowHandle(wnd);
	target = utCreateWindowTarget(hwnd);

	utSetEventHandler(onEvent);
	keepRunning = true;
	while (utPollEvents(true) && keepRunning)
	{
		tick();
	}

	utShutdown();
	return 0;
}
