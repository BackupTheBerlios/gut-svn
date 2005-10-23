/**********************************************************************
 * GameGut - x11_errors.cpp
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


/****************************************************************************
 * Called by the X11 code when an error is encountered; right now it just
 * dumps a simple message to the log. Later on I'd like to query the OS for
 * more information about the error, like I do in the Windows port.
 ****************************************************************************/

int utxReportX11Error_(const char* function, const char* file, int line)
{
	char buffer[1024];
	sprintf(buffer, "%s(%d): call to %s() failed\n ", file, line, function);
	utLog(buffer);
	return 0;
}
