/**********************************************************************
 * GameGut - gut_memory.h
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

#include <stdlib.h>

typedef void* (*utAllocHandler)(size_t size, const char* file, int line);
typedef void* (*utReallocHandler)(void* ptr, size_t size, const char* file, int line);
typedef void  (*utFreeHandler)(void* ptr, const char* file, int line);

UT_EXPORT void* utAlloc(size_t size, const char* file, int line);
UT_EXPORT void* utRealloc(void* ptr, size_t size, const char* file, int line);
UT_EXPORT void  utFree(void* ptr, const char* file, int line);
UT_EXPORT void  utSetAllocHandlers(utAllocHandler fAlloc, utReallocHandler fRealloc, utFreeHandler fFree);
UT_EXPORT int   utGetAllocCount();
UT_EXPORT int   utGetReallocCount();
UT_EXPORT int   utGetFreeCount();

#if defined(_DEBUG) || defined(DEBUG)
	#define utALLOC(size)  utAlloc(size, __FILE__, __LINE__)
	#define utALLOCT(type) (type*)utAlloc(sizeof(type), __FILE__, __LINE__)
	#define utREALLOC(ptr, size) utRealloc(ptr, size, __FILE__, __LINE__)
	#define utFREE(ptr)    utFree(ptr, __FILE__, __LINE__)
#else
	#define utALLOC(size)  utAlloc(size, NULL, 0)
	#define utALLOCT(type) (type*)utAlloc(sizeof(type), NULL, 0)
	#define utREALLOC(ptr, size) utRealloc(ptr, size, NULL, 0)
	#define utFREE(ptr)    utFree(ptr, NULL, 0)
#endif
