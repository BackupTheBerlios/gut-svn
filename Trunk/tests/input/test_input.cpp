/**********************************************************************
 * GameGut - test_input.cpp
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

#define NO_KEYBOARD_MESSAGES   0
#define NO_MOUSE_MESSAGES      0
#define NO_CTRL_MESSAGES       0

void UT_CALLBACK onEvent(utEvent* event)
{
	char buffer[512];

	switch (event->what)
	{
#if !NO_KEYBOARD_MESSAGES
	case UT_EVENT_KEY:
		sprintf_s(buffer, 512, "Keyboard(%d):Button(%d) %s\n", event->arg0, event->arg1, event->arg2 ? "DOWN" : "UP");
		utLog(buffer);
		break;

	case UT_EVENT_KEY_REPEAT:
		sprintf_s(buffer, 512, "Keyboard(%d):Button(%d) REPEAT\n", event->arg0, event->arg1);
		utLog(buffer);
		break;

	case UT_EVENT_CHAR:
		sprintf_s(buffer, 512, "Char: %c\n", event->arg1);
		utLog(buffer);
		break;
#endif
#if !NO_MOUSE_MESSAGES
	case UT_EVENT_MOUSE_AXIS:
		sprintf_s(buffer, 512, "Mouse(%d):Axis(%d) %d\n", event->arg0, event->arg1, event->arg2);
		utLog(buffer);
		break;

	case UT_EVENT_MOUSE_BUTTON:
		sprintf_s(buffer, 512, "Mouse(%d):Button(%d) %s\n", event->arg0, event->arg1, event->arg2 ? "DOWN" : "UP");
		utLog(buffer);
		break;
#endif
#if !NO_CTRL_MESSAGES
	case UT_EVENT_CTRL_AXIS:
		sprintf_s(buffer, 512, "Controller(%d):Axis(%d) %d\n", event->arg0, event->arg1, event->arg2);
		utLog(buffer);
		break;

	case UT_EVENT_CTRL_BUTTON:
		sprintf_s(buffer, 512, "Controller(%d):Button(%d) %s\n", event->arg0, event->arg1, event->arg2 ? "DOWN" : "UP");
		utLog(buffer);
		break;
#endif
	case UT_EVENT_WINDOW_CLOSE:
		utDestroyWindow(event->window);
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


int main()
{
	utSetLogHandler(onLogMessage);
	utInitialize();

	utWindow wnd = utCreateWindow("Toolkit Input Sample", 640, 480);
	if (wnd == NULL)
		die("Failed to create window 1");

	utSetEventHandler(onEvent);
	while (utPollEvents(false))
	{
		/* Update the scene... */
	}

	utShutdown();
	return 0;
}
