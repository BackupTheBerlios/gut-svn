/**********************************************************************
 * GameGut - msw_events.cpp
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
#include "msw_platform.h"


/****************************************************************************
 * Called by utPollEvents() in ut_events.cpp; send off any events that have
 * accumulated in the queues since last call.
 ****************************************************************************/

int utxPollEvents(int block)
{
	/* Process all of the user input first. If the window closes or loses
	 * focus below all of the user input will have already been handled */
	utx_msw_PollInputDevices();
	
	/* If non-blocking and no messages in queue, return immediately */
	MSG msg;
	if (!block && !PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
		return true;
	
	/* Process events until empty */
	GetMessage(&msg, NULL, 0, 0);
	do
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	} while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE));

	return true;
}


/****************************************************************************
 * The Win32 window callback for all Toolkit windows, set up by the window
 * creation code in ut_window.cpp
 ****************************************************************************/

LRESULT CALLBACK utx_msw_WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	/* Identify the Toolkit window object associated with this message. I
	 * will need this to generate a Toolkit event to send up to the host */
#if defined(GWLP_USERDATA)
	utWindow window = (utWindow)GetWindowLongPtr(hWnd, GWLP_USERDATA);
#else
	utWindow window = (utWindow)GetWindowLong(hWnd, GWL_USERDATA);
#endif
	if (window == NULL)
		return DefWindowProc(hWnd, msg, wp, lp);

	utEvent event;
	event.window = window;
	event.when   = GetMessageTime();
	event.arg0   = 0;
	event.arg1   = 0;
	event.arg2   = 0;

	switch (msg)
	{
	case WM_ACTIVATE:
		event.what = UT_EVENT_WINDOW_FOCUS;
		event.arg0 = (wp != 0) ? 1 : 0;
		utxSendWindowEvent(&event);
		break;

	case WM_CLOSE:
		event.what = UT_EVENT_WINDOW_CLOSE;
		utxSendWindowEvent(&event);
		return 0;

	case WM_PAINT:
		event.what = UT_EVENT_WINDOW_REDRAW;
		utxSendWindowEvent(&event);
		break;

	case WM_SIZE:
		event.what = UT_EVENT_WINDOW_RESIZE;
		event.arg0 = LOWORD(lp);
		event.arg1 = HIWORD(lp);
		utxSendWindowEvent(&event);
		return 0;
	}

	return DefWindowProc(hWnd, msg, wp, lp);
}
