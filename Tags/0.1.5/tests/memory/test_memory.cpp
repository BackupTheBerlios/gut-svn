/**********************************************************************
 * GameGut - test_memory.cpp
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

#include <gut/gut.h>
#include "core/array.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

static int numAllocs   = 0;
static int numReallocs = 0;
static int numFrees    = 0;
static int numCustom   = 0;


/* Logging callback - dump everything to the console */
void UT_CALLBACK logPrinter(const char* message)
{
	printf(message);
}


/* Custom memory allocation callbacks */
void* myAlloc(size_t size, const char* file, int line)
{
	numCustom++;
	return malloc(size);
}


void* myRealloc(void* ptr, size_t size, const char* file, int line)
{
	numCustom++;
	return realloc(ptr, size);
}

void myFree(void* ptr, const char* file, int line)
{
	numCustom++;
	free(ptr);
}


/* Testing primitives */
void die(const char* msg, ...)
{
	char buffer[512];
	va_list args;

	va_start(args, msg);
	vsprintf(buffer, msg, args);
	va_end(args);
	puts(buffer);
	exit(1);
}

void verify_alloc(int num)
{
	numAllocs += num;
	if (numAllocs != utGetAllocCount())
		die("*** Alloc count is %d, should be %d!\n", utGetAllocCount(), numAllocs);
}

void verify_realloc(int num)
{
	numReallocs += num;
	if (numReallocs != utGetReallocCount())
		die("*** Realloc numReallocs is %d, should be %d!\n", utGetReallocCount(), numReallocs);
}

void verify_free(int num)
{
	numFrees += num;
	if (numFrees != utGetFreeCount())
		die("*** Free numFrees is %d, should be %d!\n", utGetFreeCount(), numFrees);
}


/* Test replacement C-style allocators */
void test_c_allocators()
{
	puts("\nTesting C allocators:\n");

	void* ptr = utALLOC(32);
	printf("  ptr is %p, alloc count is %d\n", ptr, utGetAllocCount());
	ptr = utREALLOC(ptr, 64);
	printf("  realloced ptr is %p, realloc count is %d\n", ptr, utGetReallocCount());
	utFREE(ptr);
	printf("  ptr freed, free count is %d\n", utGetFreeCount());

	verify_alloc(1);
	verify_realloc(1);
	verify_free(1);
}


/* Test custom memory allocation callbacks */
void test_custom_allocators()
{
	puts("\nTesting custom allocators:\n");

	utSetAllocHandlers(myAlloc, myRealloc, myFree);
	void* ptr = utALLOC(32);
	if (numCustom != 1)  
		die("alloc callback failed");
	printf("  ptr is %p, alloc count is %d\n", ptr, utGetAllocCount());

	ptr = utREALLOC(ptr, 64);
	if (numCustom != 2)
		die("realloc callback failed");
	printf("  realloced ptr is %p, realloc count is %d\n", ptr, utGetReallocCount());

	utFREE(ptr);
	if (numCustom != 3)
		die("free callback failed");
	printf("  ptr freed, free count is %d\n", utGetFreeCount());

	verify_alloc(1);
	verify_realloc(1);
	verify_free(1);
}


/* Test the Toolkit's array class, used for internal lists of primitive types */
void test_array()
{
	puts("\nTesting Toolkit array template:\n");

	utxArray<int> arr;
	arr.push_back(0);
	printf("  added item, size is %d, capacity is %d, alloc count is %d\n", arr.size(), arr.capacity(), utGetAllocCount());
	verify_alloc(1);

	arr.push_back(1);
	printf("  added item, size is %d, capacity is %d, realloc count is %d\n", arr.size(), arr.capacity(), utGetReallocCount());
	verify_realloc(1);

	arr.push_back(2);
	printf("  added item, size is %d, capacity is %d, realloc count is %d\n", arr.size(), arr.capacity(), utGetReallocCount());
	verify_realloc(1);

	arr.push_back(3);
	printf("  added item, size is %d, capacity is %d, realloc count is %d\n", arr.size(), arr.capacity(), utGetReallocCount());
	verify_realloc(0);

	arr.clear();
	printf("  array cleared, free count is %d\n", utGetFreeCount());
	verify_free(1);
}


/* Create a memory leak and make sure an error is shown */
void test_leak()
{
	puts("\nTesting memory leak detection (should throw a memory error):\n");

	void* ptr = utALLOC(4);
	utShutdown();
}


int main()
{
	utInitialize();
	utSetLogHandler(logPrinter);

	/* Get a baseline allocation count */
	numAllocs   = utGetAllocCount();
	numReallocs = utGetReallocCount();
	numFrees    = utGetFreeCount();

	test_c_allocators();
	test_custom_allocators();
	test_array();
	test_leak();  /* must be last */
	
	puts("\nOK");
	return 0;
}
