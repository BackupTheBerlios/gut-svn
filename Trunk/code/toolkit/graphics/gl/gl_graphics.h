/**********************************************************************
 * GameGut - gl_graphics.h
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

#include "graphics/graphics.h"

#if defined(_WIN32) && !defined(FORCE_X11)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include <GL/gl.h>
#include <GL/glu.h>

#if !defined(GL_BGR_EXT)
	#define GL_BGR_EXT 0x80E0
#endif

#if !defined(GL_BGR)
	#define GL_BGR GL_BGR_EXT
#endif


struct utxIndexBuffer : Referenced
{
	virtual ~utxIndexBuffer();
	int* data;
	int size;
};


struct utxRenderTarget : Referenced
{
	virtual ~utxRenderTarget() { }
	int width, height;
};


struct utxTexture : Referenced
{
	virtual ~utxTexture();
	GLuint handle;
	int width, height;
	utTextureFormat format;
};


struct utxVertexBuffer : Referenced
{
	virtual ~utxVertexBuffer();
	float* data;
	int size;
};


struct utxVertexFormat : Referenced
{
	virtual ~utxVertexFormat();
	utVertexAttribute* attributes;
	int count;
	int stride;
};


utRenderTarget utxCreateWindowTarget(void* display, void* window);

int utxTestGLErrorFunc(const char* function, const char* file, int line);
#define utxTestGLError(func)  utxTestGLErrorFunc(func, __FILE__, __LINE__)
