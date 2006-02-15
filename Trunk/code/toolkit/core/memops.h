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

/*
#include <new>

inline void* operator new(size_t size) throw (std::bad_alloc)
{
	return utAlloc(size, NULL, 0);
}
	
inline void* operator new(size_t size, const char* file, int line) throw (std::bad_alloc)
{
	return utAlloc(size, file, line);
}

inline void operator delete(void* ptr, const char* file, int line) throw (std::bad_alloc)
{
	utFree(ptr, file, line);
}

inline void operator delete(void* ptr) throw (std::bad_alloc)
{
	utFREE(ptr);
}

*/

#define utNEW     new(__FILE__,__LINE__)
#define utDELETE  delete


