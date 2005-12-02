/**********************************************************************
 * GameGut - msw_window.cpp
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


/* A Windows specific version of the utWindow object */
struct utxWindow : utxWindowBase
{
	HWND hWnd;
};


/****************************************************************************
 * Called by utCreateWindow() in ut_window.cpp; the MS Windows specific 
 * window creation code.
 ****************************************************************************/

utWindow utxCreateWindow(const char* title, int width, int height)
{
	HINSTANCE hInstance = GetModuleHandle(NULL);

	/* Standard Win32 setup...register a window class */
	WNDCLASS wc;
	wc.style = CS_HREDRAW |	CS_VREDRAW;
	wc.lpfnWndProc = utx_msw_WndProc;
	wc.cbClsExtra =	0;
	wc.cbWndExtra =	0;
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = NULL;
	wc.hInstance = hInstance;
	wc.lpszClassName = "utgfxwnd";
	wc.lpszMenuName	= NULL;
	RegisterClass(&wc);

	/* Make the window bigger to account for borders, caption, etc. */
	DWORD style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | 
	              WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_VISIBLE;

	RECT rect;
	SetRect(&rect, 0, 0, width, height);
	AdjustWindowRect(&rect, style, FALSE);
	width = rect.right - rect.left;
	height = rect.bottom - rect.top;

	/* Now create the window */
	HWND hWnd;
	hWnd = CreateWindowEx(0, "utgfxwnd", title, style, 
	                      CW_USEDEFAULT, CW_USEDEFAULT, width, height, 
	                       NULL, NULL, hInstance, NULL);
	if (hWnd == NULL)
	{
		utxLogError("CreateWindowEx", GetLastError());
		return NULL;
	}

	/* Create a Toolkit window object */
	utWindow window = utALLOCT(utxWindow);
	window->handle = (void*)hWnd;
	window->hWnd = hWnd;

	/* Attach my object to the window for the event handler */
#if defined(GWLP_USERDATA)
	SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG)window);
#else
	SetWindowLong(hWnd, GWL_USERDATA, (LONG)window);
#endif

	/* Display the window */
	ShowWindow(hWnd, SW_SHOWNORMAL);
//	UpdateWindow(hWnd);

	return window;
}


/****************************************************************************
 * Called by utDestroyWindow() in ut_window.cpp; the MS Windows specific 
 * window teardown code.
 ****************************************************************************/

int utxDestroyWindow(utWindow window)
{
	if (window != NULL)
	{
		if (window->hWnd != NULL && IsWindow(window->hWnd))
			DestroyWindow(window->hWnd);
		utFREE(window);
	}
	return true;
}

