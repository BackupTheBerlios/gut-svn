/**********************************************************************
 * GameGut - x11_timer.cpp
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
#include <sys/time.h>


int utGetTimer()
{
	timeval systemTime;
	gettimeofday(&systemTime, NULL);
	return systemTime.tv_sec * 1000L + systemTime.tv_usec / 1000L;
}
