/**********************************************************************
 * GameGut - x11_events.cpp
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
#include <stdio.h>

static bool myKeyRepeating(XEvent* xevent);
static int  myTranslateKeycode(XEvent* event);
static void mySendCharEvent(XEvent* xevent, utEvent* event);

extern int utx_x11_keymap[];



/****************************************************************************
 * Called by utPollEvents(); empties the system message queue
 ****************************************************************************/

int utxPollEvents(int block)
{
	/* If non-blocking and there are no events available, return immediately */
	if (!block && !XPending(utx_display))
		return true;
		
	/* Empty the queue */
	do
	{
		XEvent xevent;
		XNextEvent(utx_display, &xevent);

		/* Find the window associated with this event. I'll need it in order
		 * to populate a utEvent to send back to the host */ 
		utWindow window = utFindWindowByHandle((void*)xevent.xany.window);
		if (window == NULL)
			continue;

		utEvent event;
		event.window = window;
		event.when   = 0;
		event.arg0   = 0;
		event.arg1   = 0;
		event.arg2   = 0;
		
		switch (xevent.type)
		{
		case Expose:
			event.what = UT_EVENT_WINDOW_REDRAW;
			utxSendWindowEvent(&event);
			break;

		case ClientMessage:
			if ((Atom)(xevent.xclient.data.l[0]) == utx_wmDeleteAtom)
			{
				event.what = UT_EVENT_WINDOW_CLOSE;
				utxSendWindowEvent(&event);
			}
			break;

		case ConfigureNotify:
			event.what = UT_EVENT_WINDOW_RESIZE;
			event.arg0 = xevent.xconfigure.width;
			event.arg1 = xevent.xconfigure.height;
			if (event.arg0 != window->width || event.arg1 != window->height)
			{
				window->width = event.arg0;
				window->height = event.arg1;
				utxSendWindowEvent(&event);
			}	
			break;

		case FocusIn:
			event.what = UT_EVENT_WINDOW_FOCUS;
			event.arg0 = 1;
			utxSendWindowEvent(&event);
			break;
			
		case FocusOut:
			event.what = UT_EVENT_WINDOW_FOCUS;
			event.arg0 = 0;
			utxSendWindowEvent(&event);
			break;

		case KeyPress:
			event.what = UT_EVENT_KEY;
			event.when = ((XKeyEvent*)(&xevent))->time;
			event.arg0 = 0;
			event.arg1 = myTranslateKeycode(&xevent);
			event.arg2 = 1;
			utxSendInputEvent(&event);
			mySendCharEvent(&xevent, &event);
			break;

		case KeyRelease:
			/* X autorepeat works by sending additional key release/press
			 * pairs, to make it look like the key is getting tapped. */
			event.when = ((XKeyEvent*)(&xevent))->time;
			event.arg0 = 0;
			event.arg1 = myTranslateKeycode(&xevent);
			if (myKeyRepeating(&xevent))
			{
				event.what = UT_EVENT_KEY_REPEAT;
				event.arg2 = 1;
				utxSendInputEvent(&event);
				mySendCharEvent(&xevent, &event);
			}
			else
			{
				event.what = UT_EVENT_KEY;
				event.arg2 = 0;
				utxSendInputEvent(&event);
			}
			break;
		}
	} while (XPending(utx_display));
	
	return true;
}


/****************************************************************************
 * Called by utxInputFocusChanged() in ut_input.cpp when the user has
 * switched to a different application. Unused for this platform.
 ****************************************************************************/

int utxResetInputPlatform()
{
	return true;
}


/****************************************************************************
 * Helper function to convert an X11 keycode into a Toolkit keycode
 ****************************************************************************/

static int myTranslateKeycode(XEvent* xevent)
{
	/* Start by getting a keysym */
	XKeyEvent* event = (XKeyEvent*)xevent;
	KeySym keysym = XLookupKeysym(event, 0);

	if (keysym == 0)
	{
		switch (event->keycode)
		{
		case 0x73:
			return UT_KEY_LEFTSYSTEM;
		case 0x74:
			return UT_KEY_RIGHTSYSTEM;
		case 0x75:
			return UT_KEY_MENU;
		}
	}

	/* Convert the keysym to a Toolkit keycode */
	if (keysym & 0xff00)
		return (utx_x11_keymap[keysym & 0x00ff] >> 16);
	else
		return (utx_x11_keymap[keysym] & 0xffff);
}


/****************************************************************************
 * Called when a key release event is detected; checks to see if it is
 * immediately followed by a key press, which is how X11 does autorepeat
 ****************************************************************************/

bool myKeyRepeating(XEvent* xevent)
{
	/* XPeekEvent will block if there are no messages waiting */
	if (XPending(utx_display))
	{
		/* Get the next event without actually removing it from the queue */
		XEvent next;
		XPeekEvent(utx_display, &next);

		/* Do the two messages match up? */
		if (next.type == KeyPress && next.xkey.keycode == xevent->xkey.keycode && next.xkey.time == xevent->xkey.time)
		{
			/* It's a repeat, remove the event from the queue */
			XNextEvent(utx_display, &next);
			return true;
		}
	}

	return false;
}


/****************************************************************************
 * Translates a keycode into a ASCII character and sends a char event. I
 * have a feeling this is woefully insufficient but it is all I found
 ****************************************************************************/

void mySendCharEvent(XEvent* xevent, utEvent* event)
{
	char buffer[2];
	if (XLookupString((XKeyEvent*)xevent, buffer, sizeof(buffer), 0, 0) > 0)
	{
		event->what = UT_EVENT_CHAR;
		event->arg0 = buffer[0];
		event->arg1 = 0;
		event->arg2 = 0;
		utxSendInputEvent(event);
	}
}

