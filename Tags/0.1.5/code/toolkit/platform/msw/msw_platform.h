/**********************************************************************
 * GameGut - msw_platform.h
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

#define WIN32_LEAN_AND_MEAN
#include <windows.h>


/* Error reporting */
int utx_msw_ReportErrorFunc(const char* function, HRESULT code, const char* file, int line);
#define utx_msw_ReportError(func, code)  utx_msw_ReportErrorFunc(func, code, __FILE__, __LINE__)


/* Window management */
LRESULT CALLBACK utx_msw_WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);


/* User input handling */
int utx_msw_InitializeInput();
int utx_msw_ShutdownInput();
int utx_msw_PollInputDevices();
