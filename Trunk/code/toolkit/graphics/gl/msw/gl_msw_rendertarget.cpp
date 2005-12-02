/**********************************************************************
 * GameGut - gl_msw_rendertarget.cpp
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
#include "core/msw/errors.h"
#include "../gl_graphics.h"

static LRESULT CALLBACK MswRenderTargetWndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);


struct utxMswRenderTarget : utxRenderTarget
{
	HWND  hWnd;
	HDC   hDC;
	HGLRC hRC;

	~utxMswRenderTarget()
	{
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(hRC);
		ReleaseDC(hWnd, hDC);
		DestroyWindow(hWnd);
	}
};



utRenderTarget utxCreateWindowTarget(void* window)
{
	HINSTANCE hInstance = GetModuleHandle(NULL);
	HWND frameWnd = (HWND)window;

	/* Determine the size of the frame window client area */
	RECT rect;
	GetClientRect(frameWnd, &rect);
	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;

	/* Create a child window to fill that area */
	WNDCLASS wc;
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = MswRenderTargetWndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = "UtGlWnd";
	RegisterClass(&wc);

	HWND hWnd = CreateWindowEx(WS_EX_APPWINDOW, "UtGlWnd", NULL, WS_CHILD | WS_VISIBLE,
	                           0, 0, width, height, frameWnd, NULL, hInstance, NULL);
	if (hWnd == NULL)
	{
		utxLogError("CreateWindowEx", GetLastError());
		return NULL;
	}

	/* Get a drawing context for the window */
	HDC hDC = GetDC(hWnd);
	if (hDC == NULL) 
	{
		utxLogError("GetDC", GetLastError());
		DestroyWindow(hWnd);
		return NULL;
	}

	/* For the time being I am going to hardcode a pixel format. Eventually I
	 * will need an API to specify the required capabilities */
	PIXELFORMATDESCRIPTOR pfd;
	ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize       = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion    = 1;
	pfd.dwFlags     = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER | PFD_TYPE_RGBA;
	pfd.cColorBits  = 16;
	pfd.cDepthBits  = 32;
	pfd.dwLayerMask = PFD_MAIN_PLANE;

	int format  = ChoosePixelFormat(hDC, &pfd);
	if (format == 0) 
	{
		utxLogError("ChoosePixelFormat", GetLastError());
		ReleaseDC(hWnd, hDC);
		DestroyWindow(hWnd);
		return NULL;
	}

	DescribePixelFormat(hDC, format, sizeof(pfd), &pfd);
	if (!SetPixelFormat(hDC, format, &pfd)) 
	{
		utxLogError("SetPixelFormat", GetLastError());
		ReleaseDC(hWnd, hDC);
		DestroyWindow(hWnd);
		return NULL;
	}

	/* Create a rendering context */
	HGLRC hRC = wglCreateContext(hDC);
	if (hRC == NULL) 
	{
		utxLogError("wglCreateContext", GetLastError());
		ReleaseDC(hWnd, hDC);
		DestroyWindow(hWnd);
		return NULL;
	}

	wglMakeCurrent(hDC, hRC);

	/* All set */
	utxMswRenderTarget* target = utNEW utxMswRenderTarget;
	target->hWnd   = hWnd;
	target->hDC    = hDC;
	target->hRC    = hRC;
	target->width  = width;
	target->height = height;

	utResizeRenderTarget(target, width, height);
	return target;
}


int utResizeRenderTarget(utRenderTarget target, int width, int height)
{
	utxMswRenderTarget* mst = (utxMswRenderTarget*)target;
	mst->width  = width;
	mst->height = height;
	MoveWindow(mst->hWnd, 0, 0, width, height, FALSE);
	glViewport(0, 0, width, height);
	return true;
}


int utSwapRenderTarget(utRenderTarget target)
{
	utxMswRenderTarget* mst = (utxMswRenderTarget*)target;
	::SwapBuffers(mst->hDC);
	return true;
}


LRESULT CALLBACK MswRenderTargetWndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	return DefWindowProc(hWnd, msg, wp, lp);
}

