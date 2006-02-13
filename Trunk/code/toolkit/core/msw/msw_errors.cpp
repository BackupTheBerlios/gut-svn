/**********************************************************************
 * GameGut - core/msw/msw_errors.cpp
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
 * Called by the Win32 code when an error is encountered; tries to pull as
 * much information about the error as it can from Windows then formats a
 * message and prints it to the log.
 ****************************************************************************/

int utxLogErrorFunc(const char* function, HRESULT code, const char* file, int line)
{
	char buffer[8192];
	sprintf_s(buffer, 8192, "%s(%d): call to %s() failed with error code %x:\n", file, line, function, code);
	utLog(buffer);

	char* errorMessage;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
	              NULL, code, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                 (LPTSTR)&errorMessage, 0, NULL);
	sprintf_s(buffer, 8192, "%s\n", errorMessage);
	LocalFree(errorMessage);
	utLog(buffer);
	return 0;
}
