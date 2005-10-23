/**********************************************************************
 * GameGut - gut_platform.h
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

enum utInputDeviceKind
{
	UT_DEVICE_NONE,
	UT_DEVICE_KEYBOARD,
	UT_DEVICE_MOUSE,
	UT_DEVICE_CONTROLLER
};

enum utEventKind
{
	UT_EVENT_NONE,
	UT_EVENT_WINDOW_CLOSE,
	UT_EVENT_WINDOW_REDRAW,
	UT_EVENT_WINDOW_RESIZE,
	UT_EVENT_WINDOW_FOCUS,
	UT_EVENT_KEY,
	UT_EVENT_KEY_REPEAT,
	UT_EVENT_CHAR
};

enum utKeycode
{
	UT_KEY_UNKNOWN,
	UT_KEY_ESCAPE,
	UT_KEY_1,
	UT_KEY_2,
	UT_KEY_3,
	UT_KEY_4,
	UT_KEY_5,
	UT_KEY_6,
	UT_KEY_7,
	UT_KEY_8,
	UT_KEY_9,
	UT_KEY_0,
	UT_KEY_MINUS,
	UT_KEY_EQUALS,
	UT_KEY_BACKSPACE,
	UT_KEY_TAB,
	UT_KEY_Q,
	UT_KEY_W,
	UT_KEY_E,
	UT_KEY_R,
	UT_KEY_T,
	UT_KEY_Y,
	UT_KEY_U,
	UT_KEY_I,
	UT_KEY_O,
	UT_KEY_P,
	UT_KEY_LEFTBRACKET,
	UT_KEY_RIGHTBRACKET,
	UT_KEY_ENTER,
	UT_KEY_LEFTCONTROL,
	UT_KEY_A,
	UT_KEY_S,
	UT_KEY_D,
	UT_KEY_F,
	UT_KEY_G,
	UT_KEY_H,
	UT_KEY_J,
	UT_KEY_K,
	UT_KEY_L,
	UT_KEY_SEMICOLON,
	UT_KEY_APOSTROPHE,
	UT_KEY_GRAVE,
	UT_KEY_LEFTSHIFT,
	UT_KEY_BACKSLASH,
	UT_KEY_Z,
	UT_KEY_X,
	UT_KEY_C,
	UT_KEY_V,
	UT_KEY_B,
	UT_KEY_N,
	UT_KEY_M,
	UT_KEY_COMMA,
	UT_KEY_PERIOD,
	UT_KEY_SLASH,
	UT_KEY_RIGHTSHIFT,
	UT_KEY_NUMPADMULTIPLY,
	UT_KEY_LEFTALT,
	UT_KEY_SPACE,
	UT_KEY_CAPSLOCK,
	UT_KEY_F1,
	UT_KEY_F2,
	UT_KEY_F3,
	UT_KEY_F4,
	UT_KEY_F5,
	UT_KEY_F6,
	UT_KEY_F7,
	UT_KEY_F8,
	UT_KEY_F9,
	UT_KEY_F10,
	UT_KEY_NUMLOCK,
	UT_KEY_SCROLLLOCK,
	UT_KEY_NUMPAD7,
	UT_KEY_NUMPAD8,
	UT_KEY_NUMPAD9,
	UT_KEY_NUMPADMINUS,
	UT_KEY_NUMPAD4,
	UT_KEY_NUMPAD5,
	UT_KEY_NUMPAD6,
	UT_KEY_NUMPADPLUS,
	UT_KEY_NUMPAD1,
	UT_KEY_NUMPAD2,
	UT_KEY_NUMPAD3,
	UT_KEY_NUMPAD0,
	UT_KEY_NUMPADDECIMAL,
	UT_KEY_OEM102,
	UT_KEY_F11,
	UT_KEY_F12,
	UT_KEY_F13 = 0x64,
	UT_KEY_F14,
	UT_KEY_F15,
	UT_KEY_KANA = 0x70,
	UT_KEY_ABNTC1 = 0x73,
	UT_KEY_CONVERT = 0x79,
	UT_KEY_NOCONVERT = 0x7b,
	UT_KEY_YEN = 0x7d,
	UT_KEY_ABNTC2,
	UT_KEY_NUMPADEQUALS = 0x8d,
	UT_KEY_PREVTRACK = 0x90,
	UT_KEY_AT,
	UT_KEY_COLON,
	UT_KEY_UNDERLINE,
	UT_KEY_KANJI,
	UT_KEY_STOP,
	UT_KEY_AX,
	UT_KEY_UNLABELED,
	UT_KEY_NEXTTRACK = 0x99,
	UT_KEY_NUMPADENTER = 0x9c,
	UT_KEY_RIGHTCONTROL,
	UT_KEY_MUTE = 0xa0,
	UT_KEY_CALCULATOR,
	UT_KEY_PLAYPAUSE,
	UT_KEY_MEDIASTOP = 0xa4,
	UT_KEY_VOLUMEDOWN = 0xae,
	UT_KEY_VOLUMEUP = 0xb0,
	UT_KEY_WEBHOME = 0xb2,
	UT_KEY_NUMPADCOMMA,
	UT_KEY_NUMPADDIVIDE = 0xb5,
	UT_KEY_SYSRQ = 0xb7,
	UT_KEY_RIGHTALT,
	UT_KEY_PAUSE = 0xc5,
	UT_KEY_HOME = 0xc7,
	UT_KEY_UP,
	UT_KEY_PAGEUP,
	UT_KEY_LEFT = 0xcb,
	UT_KEY_RIGHT = 0xcd,
	UT_KEY_END = 0xcf,
	UT_KEY_DOWN,
	UT_KEY_PAGEDOWN,
	UT_KEY_INSERT,
	UT_KEY_DELETE,
	UT_KEY_LEFTSYSTEM = 0xdb,
	UT_KEY_RIGHTSYSTEM,
	UT_KEY_MENU,
	UT_KEY_POWER,
	UT_KEY_SLEEP,
	UT_KEY_WAKE = 0xe3,
	UT_KEY_WEBSEARCH = 0xe5,
	UT_KEY_WEBFAVORITES,
	UT_KEY_WEBREFRESH,
	UT_KEY_WEBSTOP,
	UT_KEY_WEBFORWARD,
	UT_KEY_WEBBACK,
	UT_KEY_MYCOMPUTER,
	UT_KEY_MAIL,
	UT_KEY_MEDIASELECT,
};

struct utxWindow;
typedef struct utxWindow* utWindow;

typedef struct utEvent
{
	int      what;
	utWindow window;
	unsigned when;
	int      arg0;
	int      arg1;
	int      arg2;
} utEvent;

typedef void (UT_CALLBACK* utEventHandler)(utEvent*);


UT_EXPORT utWindow utCreateWindow(const char* title, int width, int height);
UT_EXPORT int      utDestroyWindow(utWindow window);
UT_EXPORT utWindow utFindWindowByHandle(void* handle);
UT_EXPORT utWindow utGetActiveWindow();
UT_EXPORT int      utGetTimer();
UT_EXPORT void*    utGetWindowHandle(utWindow window);
UT_EXPORT int      utNumControllers();
UT_EXPORT int      utNumKeyboards();
UT_EXPORT int      utNumMice();
UT_EXPORT int      utNumWindows();
UT_EXPORT int      utPollEvents(int block);
UT_EXPORT int      utSendEvent(utEvent* event);
UT_EXPORT void     utSetEventHandler(utEventHandler callback);

