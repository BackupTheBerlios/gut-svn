/**********************************************************************
 * GameGut - msw_input_fb.cpp
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

/* User input handling is spread over several levels. This is layer 
 * uses the Win32 API to implement the Toolkit API. See also the files
 * ut_input.cpp, msw_input.cpp, and msw_input_di.cpp */

#include "core/core.h"
#include "msw_platform.h"

extern int utx_msw_keymap[];

struct utx_msw_FallbackInputHandler
{
	int Initialize()
	{
		utLog("Initializing Win32 fallback user input driver\n");
		
		/* I just assume that a keyboard is attached */
		utLog("Found keyboard\n");
		utxRegisterInputDevice(UT_DEVICE_KEYBOARD, 255);

		return true;
	}


	int Shutdown()
	{
		utLog("Shutting down Win32 fallback user input driver\n");
		return true;
	}


	int HandleEvent(utEvent& event, UINT msg, WPARAM wp, LPARAM lp)
	{
		switch (msg)
		{
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
			event.what = (lp & 0x40000000) ? UT_EVENT_KEY_REPEAT : UT_EVENT_KEY;
			event.arg0 = 0;
			event.arg1 = Translate(lp);
			event.arg2 = 1;
			utSendEvent(&event);
			break;

		case WM_KEYUP:
		case WM_SYSKEYUP:
			event.what = UT_EVENT_KEY;
			event.arg0 = 0;
			event.arg1 = Translate(lp);
			event.arg2 = 0;
			utSendEvent(&event);
			break;

		case WM_CHAR:
			event.what = UT_EVENT_CHAR;
			event.arg0 = wp;
			event.arg1 = 0;
			event.arg2 = 0;
			utSendEvent(&event);
			break;

		}
		return 0;
	}


	int Translate(int keycode)
	{
		/* Pull out the OEM scan code */
		keycode = ((keycode & 0xFF0000) >> 16) | ((keycode & 0x1000000) >> 17);

		/* For some inexplicable reason, DirectInput swaps the scan codes for
		 * the [Pause] and [NumLock] keys, while everything else is the same */
		if (keycode == 0xC5)
			keycode = 0x45;
		else if (keycode == 0x45)
			keycode = 0xC5;

		return utx_msw_keymap[keycode];
	}
};
