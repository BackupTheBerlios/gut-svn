/**********************************************************************
 * GameGut - ut_array.cpp
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


static inline int nextPowerOfTwo(int num)
{
	int i = 1;
	while (i < num) i <<= 1;
	return i;
}


utxArrayBase::utxArrayBase()
{
	my_size = 0;
	my_capacity = 0;
	my_contents = NULL;
}


utxArrayBase::~utxArrayBase()
{
	if (my_contents != NULL)
		utFREE(my_contents);
}


void utxArrayBase::clear()
{
	if (my_contents != NULL)
		utFREE(my_contents);
	my_contents = NULL;
	my_size = 0;
	my_capacity = 0;
}


void utxArrayBase::grow(int elementSize)
{
	my_capacity = nextPowerOfTwo(my_capacity + 1);
	if (my_contents != NULL)
		my_contents = utREALLOC(my_contents, my_capacity * elementSize);
	else
		my_contents = utALLOC(my_capacity * elementSize);
}
