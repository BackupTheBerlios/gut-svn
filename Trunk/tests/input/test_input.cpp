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


void UT_CALLBACK onEvent(utEvent* event)
{
	char buffer[512];

	switch (event->what)
	{
	case UT_EVENT_KEY:
		sprintf(buffer, "Keyboard(%d):Button(%d) %s\n", event->arg0, event->arg1, event->arg2 ? "DOWN" : "UP");
		utLog(buffer);
		break;

	case UT_EVENT_KEY_REPEAT:
		sprintf(buffer, "Keyboard(%d):Button(%d) REPEAT\n", event->arg0, event->arg1);
		utLog(buffer);
		break;

	case UT_EVENT_CHAR:
		sprintf(buffer, "Char: %c\n", event->arg1);
		utLog(buffer);
		break;

	case UT_EVENT_MOUSE_AXIS:
		sprintf(buffer, "Mouse(%d):Axis(%d) %d\n", event->arg0, event->arg1, event->arg2);
		utLog(buffer);
		break;

	case UT_EVENT_MOUSE_BUTTON:
		sprintf(buffer, "Mouse(%d):Button(%d) %s\n", event->arg0, event->arg1, event->arg2 ? "DOWN" : "UP");
		utLog(buffer);
		break;

	case UT_EVENT_CTRL_AXIS:
		sprintf(buffer, "Controller(%d):Axis(%d) %d\n", event->arg0, event->arg1, event->arg2);
		utLog(buffer);
		break;

	case UT_EVENT_CTRL_BUTTON:
		sprintf(buffer, "Controller(%d):Button(%d) %s\n", event->arg0, event->arg1, event->arg2 ? "DOWN" : "UP");
		utLog(buffer);
		break;

	case UT_EVENT_WINDOW_CLOSE:
		utDestroyWindow(event->window);
		break;
	}
}


void UT_CALLBACK onLogMessage(const char* message)
{
	printf(message);
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

	utWindow wnd1 = utCreateWindow("Toolkit Input Sample", 640, 480);
	if (wnd1 == NULL)
		die("Failed to create window 1");

	utWindow wnd2 = utCreateWindow("Toolkit Input Sample", 640, 480);
	if (wnd2 == NULL)
		die("Failed to create window 2");

	utSetEventHandler(onEvent);
	while (utPollEvents(true))
	{
		/* Update the scene... */
	}

	utShutdown();
	return 0;
}
