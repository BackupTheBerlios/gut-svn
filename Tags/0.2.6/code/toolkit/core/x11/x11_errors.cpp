/**********************************************************************
 * GameGut - core/x11/x11_errors.cpp
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

#include <stdio.h>
#include "core/core.h"
#include "errors.h"


/****************************************************************************
 * Called by the X11 code when an error is encountered; I need to try to
 * pull more information about the error from the OS. See the MSW version.
 ****************************************************************************/

int utxLogErrorFunc(const char* function, const char* file, int line)
{
	char buffer[8192];
	sprintf(buffer, "%s(%d): call to %s() failed\n", file, line, function);
	utLog(buffer);
	return 0;
}
