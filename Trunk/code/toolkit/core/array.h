/**********************************************************************
 * GameGut - array.h
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

#include <string.h>


/***********************************************************************
 * A simple contiguous-memory array, used for keeping track of child
 * items within the toolkit. Intended for the storage of primitive 
 * data types, such as pointers and ints.
 *
 * I was originally using std::vector<> for this, but I had trouble
 * intercepting the STL memory allocations under GCC, which I need to
 * do in order to route everything through utAlloc() and utFree(). In
 * addition, the GCC STL does not release memory until the process
 * exits, so you can't reset your environment by calling utShutdown().
 **********************************************************************/

class UT_DECLSPEC utxArrayBase
{
protected:
	int    my_size;
	int    my_capacity;
	void*  my_contents;

	void   grow(int elementSize);

public:
	utxArrayBase();
	virtual ~utxArrayBase();

	void clear();
	
	inline int size() const
	{
		return my_size;
	}

	inline int capacity() const
	{
		return my_capacity;
	}
};

template<class T>
class utxArray : public utxArrayBase
{
public:
	T operator[] (int i) const 
	{ 
		return ((T*)my_contents)[i]; 
	}

	void push_back(const T item)
	{
		if (my_size == my_capacity)
			grow(sizeof(T));
		((T*)my_contents)[my_size] = item;
		my_size++;
	}

	void remove(const T item)
	{
		for (int i = 0; i < my_size; ++i)
		{
			if (((T*)my_contents)[i] == item)
			{
				int num = my_size - i - 1;
				if (num > 0)
					memmove(((T*)my_contents) + i, ((T*)my_contents) + i + 1, num * sizeof(T));
				my_size--;
			}
		}
	}
};
