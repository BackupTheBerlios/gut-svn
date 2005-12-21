/**********************************************************************
 * GameGut - ut_memory.cpp
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


/* Use the MSVC debug heap functions if available */
#if defined(_MSC_VER) && defined(_DEBUG)
	#define CRTDBG_MAP_ALLOC
	#include <stdio.h>
	#include <stdlib.h>
	#include <crtdbg.h>

	static int crtMemoryReporter(int reportType, char* userMessage, int* retVal)
	{
		utLog(userMessage);
		*retVal = 0;
		return(false);
	}
#endif


/* Memory callback functions */
static utAllocHandler   my_allocHandler   = NULL;
static utReallocHandler my_reallocHandler = NULL;
static utFreeHandler    my_freeHandler    = NULL;

/* Memory metrics */
static int my_allocCount   = 0;
static int my_reallocCount = 0;
static int my_freeCount    = 0;


int utxInitializeMemory()
{
	return true;
}


int utxShutdownMemory()
{
#if defined(CRTDBG_MAP_ALLOC)
	_CrtSetReportHook(crtMemoryReporter);
	_CrtDumpMemoryLeaks();
#endif

	if (my_allocCount != my_freeCount)
	{
		char buffer[512];
		sprintf(buffer, "Memory allocation mismatch: %d allocs, %d frees\n", my_allocCount, my_freeCount); 
		utLog(buffer);
		return false;
	}

	return true;
}


void utSetAllocHandlers(utAllocHandler fAlloc, utReallocHandler fRealloc, utFreeHandler fFree)
{
	my_allocHandler = fAlloc;
	my_reallocHandler = fRealloc;
	my_freeHandler = fFree;
}


void* utAlloc(size_t size, const char* file, int line)
{
	my_allocCount++;
	if (my_allocHandler)
		return my_allocHandler(size, file, line);

#if defined(CRTDBG_MAP_ALLOC)
	return _malloc_dbg(size, _NORMAL_BLOCK, file, line);
#else
	return malloc(size);
#endif
}


void* utRealloc(void* ptr, size_t size, const char* file, int line)
{
	my_reallocCount++;
	if (my_reallocHandler)
		return my_reallocHandler(ptr, size, file, line);

#if defined(CRTDBG_MAP_ALLOC)
	return _realloc_dbg(ptr, size, _NORMAL_BLOCK, file, line);
#else
	return realloc(ptr, size);
#endif
}


void utFree(void* ptr, const char* file, int line)
{
	my_freeCount++;
	if (my_freeHandler)
	{
		my_freeHandler(ptr, file, line);
		return;
	}

#if defined(CRTDBG_MAP_ALLOC)
	_free_dbg(ptr, _NORMAL_BLOCK);
#else
	free(ptr);
#endif
}


int utGetAllocCount()
{
	return my_allocCount;
}


int utGetReallocCount()
{
	return my_reallocCount;
}


int utGetFreeCount()
{
	return my_freeCount;
}
