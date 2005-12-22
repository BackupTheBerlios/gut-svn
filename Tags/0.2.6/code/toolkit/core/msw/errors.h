/**********************************************************************
 * GameGut - core/msw/errors.h
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

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

int utxLogErrorFunc(const char* function, HRESULT code, const char* file, int line);
#define utxLogError(func, code)  utxLogErrorFunc(func, code, __FILE__, __LINE__)
