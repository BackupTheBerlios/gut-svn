/**********************************************************************
 * GameGut - ut_string.cpp
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

#include "core.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>


#if !defined(_MSC_VER) || (_MSC_VER < 1400)

int sprintf_s(char* buffer, size_t len, const char* format, ...)
{
	va_list args;
	va_start(args, format);
	vsprintf(buffer, format, args);
	va_end(args);
	return 0;
}

int vsprintf_s(char* buffer, size_t len, const char* format, va_list args)
{
	return vsprintf(buffer, format, args);
}

#endif
