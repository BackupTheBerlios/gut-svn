/**********************************************************************
 * GameGut - gut.h
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

#if !defined(GAMEGUT_H_)
#define GAMEGUT_H_

#define GUT_VERSION      "0.1.4"

#if defined(_WIN32)
	#if defined(GUT_DLL)
		#define UT_DECLSPEC __declspec(dllexport)
	#else
		#define UT_DECLSPEC __declspec(dllimport)
	#endif
	#define UT_CALLBACK    __stdcall
#else
	#define UT_DECLSPEC
	#define UT_CALLBACK 
#endif

#define UT_EXPORT         extern "C" UT_DECLSPEC


/* Major subsystems */
UT_EXPORT int utInitialize();
UT_EXPORT int utShutdown();

#include "gut_memory.h"
#include "gut_logging.h"
#include "gut_platform.h"

#endif
