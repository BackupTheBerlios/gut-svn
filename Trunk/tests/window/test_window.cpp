/**********************************************************************
 * GameGut - test_window.cpp
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

static utWindow wnd1 = NULL;
static utWindow wnd2 = NULL;


void UT_CALLBACK onEvent(utEvent* event)
{
	char message[1024];

	int which = (event->window == wnd1) ? 1 : 2;

	switch (event->what)
	{
	case UT_EVENT_WINDOW_FOCUS:
		sprintf(message, "Window #%d focus is %d\n", which, event->arg0);
		utLog(message);
		break;

	case UT_EVENT_WINDOW_CLOSE:
		sprintf(message, "Window #%d is closing\n", which);
		utLog(message);
		utDestroyWindow(event->window);
		break;

	case UT_EVENT_WINDOW_REDRAW:
		sprintf(message, "Window #%d needs redraw\n", which);
		utLog(message);
		break;

	case UT_EVENT_WINDOW_RESIZE:
		sprintf(message, "Resize window #%d: %dx%d\n", which, event->arg0, event->arg1);
		utLog(message);
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
	/* Initialize the Toolkit */
	utInitialize();
	utSetLogHandler(onLogMessage);

	/* Open a window */
	wnd1 = utCreateWindow("Toolkit Test Window #1", 640, 480);
	if (wnd1 == NULL)
		die("Failed to create first window");

	/* Open another window */
	wnd2 = utCreateWindow("Toolkit Test Window #2", 512, 480);
	if (wnd2 == NULL)
		die("Failed to create second window");

	/* Run the event loop until both windows have closed */
	utSetEventHandler(onEvent);
	while (utPollEvents(true))
	{
		/* Update the scene... */
	}

	utShutdown();
	return 0;
}
