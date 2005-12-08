/**********************************************************************
 * GameGut - graphics/gl/gl_vertexbuffer.cpp
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
#include "gl_graphics.h"

/* Keep track of all open buffers so I can close them at exit */
static utxArray<utVertexBuffer> my_buffers;


utVertexBuffer utCreateVertexBuffer(int size, utBufferFlags flags)
{
	utVertexBuffer buffer = utNEW utxVertexBuffer();
	buffer->data = (float*)utALLOC(sizeof(float) * size);
	buffer->size = size;

	/* Add the buffer to the master list */
	buffer->reference();
	my_buffers.push_back(buffer);
	return buffer;
}


utxVertexBuffer::~utxVertexBuffer()
{
	utFREE(data);
}


void utxReleaseAllVertexBuffers()
{
	for (int i = 0; i < my_buffers.size(); ++i)
		my_buffers[i]->release();
	my_buffers.clear();
}	


int utCopyVertexData(utVertexBuffer buffer, const float* data, int size)
{
	if (size > buffer->size)
	{
		utLog("utCopyVertices: data size is greater than buffer size\n");
		return false;
	}

	memcpy(buffer->data, data, size * sizeof(float));
	return true;
}
