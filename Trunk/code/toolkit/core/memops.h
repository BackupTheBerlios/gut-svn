/**********************************************************************
 * GameGut - memops.h
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

/***********************************************************************
 * A wrapper to connect the C++ memory operators to the toolkit 
 * allocator functions.
 **********************************************************************/

inline void* operator new(size_t size, const char* file, int line)
{
	return utAlloc(size, file, line);
}

inline void operator delete(void* ptr, const char* file, int line)
{
	utFree(ptr, file, line);
}

inline void operator delete(void* ptr)
{
	utFREE(ptr);
}


#if defined(_DEBUG)
	#define utNEW     new(__FILE__,__LINE__)
	#define utDELETE  delete
#else
	#define utNEW  new
	#define utDELETE
#endif

