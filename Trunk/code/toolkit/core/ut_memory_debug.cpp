/**********************************************************************
 * GameGut - ut_memory_debug.cpp
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
#include "core.h"

/* A rudimentary memory tracking system, thrown together to help identify
 * memory leaks in the X11 port. I have a real tracking system somewhere,
 * based on Paul Nettle's FlipCode articles. If there is any demand I will
 * dig it up and hook it into the Toolkit */

typedef struct MyAllocInfo
{
	void* address;
	const char* filename;
	int fileline;
} MyAllocInfo;

static const int MAX_ALLOCS = 1000;
static MyAllocInfo my_allocs[MAX_ALLOCS];
static int my_nextAlloc;

static void* MyDebugAlloc(size_t size, const char* file, int line);
static void* MyDebugRealloc(void* ptr, size_t size, const char* file, int line);
static void  MyDebugFree(void* ptr, const char* file, int line);


void utEnableMemoryDebugging()
{
	utSetAllocHandlers(MyDebugAlloc, MyDebugRealloc, MyDebugFree);
	my_nextAlloc = 0;
}


void utShowMemoryReport()
{
	int count = 0;
	for (int i = 0; i < my_nextAlloc; ++i)
	{
		if (my_allocs[i].address != NULL)
		{
			printf("%s(%d): memory leak detected\n", my_allocs[i].filename, my_allocs[i].fileline);
			count++;
		}
	}

	char msg[512];
	sprintf(msg, "%d leaks detected\n", count);
	utLog(msg);
}


/**********************************************************************
 * Debug memory operators
 **********************************************************************/

static int MyIndexOf(void* address)
{
	for (int i = 0; i < my_nextAlloc; ++i)
	{
		if (my_allocs[i].address == address)
			return i;
	}
	return -1;
}

static void* MyDebugAlloc(size_t size, const char* file, int line)
{
	void* address = malloc(size);

	if (my_nextAlloc < MAX_ALLOCS)
	{
		my_allocs[my_nextAlloc].address = address;
		my_allocs[my_nextAlloc].filename = file;
		my_allocs[my_nextAlloc].fileline = line;
		my_nextAlloc++;
	}

	return address;
}

static void* MyDebugRealloc(void* ptr, size_t size, const char* file, int line)
{
	void* address = realloc(ptr, size);

	int index = MyIndexOf(ptr);
	if (index >= 0)
		my_allocs[index].address = address;

   return address ;
}

static void MyDebugFree(void* ptr, const char* file, int line)
{
	free(ptr);
	int index = MyIndexOf(ptr);
		my_allocs[index].address = NULL;
}

