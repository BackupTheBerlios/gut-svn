/**********************************************************************
 * GameGut - graphics/gl/gl_graphics.cpp
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

/* Render matrices */
static float my_projMatrix[16];
static float my_viewMatrix[16];
static float my_modelMatrix[16];


int utxInitializeGraphics()
{
	/* Initialize all rendering matrices to identity */
	utMatrix4SetIdentity(my_projMatrix);
	utMatrix4SetIdentity(my_viewMatrix);
	utMatrix4SetIdentity(my_modelMatrix);
	return true;
}


int utxShutdownGraphics()
{
	utxReleaseAllIndexBuffers();
	utxReleaseAllVertexBuffers();
	utxReleaseAllRenderTargets();
	return true;
}


int utBeginFrame()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	return true;
}


int utClear(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	return true;
}


int utDraw(utVertexBuffer vertices, utIndexBuffer indices, int start, int count)
{
	int stride = 0;
	
	if (count == UT_DRAW_ALL)
		count = indices->size;

	glEnableClientState(GL_VERTEX_ARRAY);

	glVertexPointer(3, GL_FLOAT, stride, vertices->data);

	glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, indices->data);

	glDisableClientState(GL_VERTEX_ARRAY);
	return true;
}


int utEndFrame()
{
	return true;
}


int utSetRenderMatrix(utRenderMatrix which, const float* matrix)
{
	switch (which)
	{
	case UT_MATRIX_PROJECTION:
		utMatrix4Copy(my_projMatrix, matrix);
		glMatrixMode(GL_PROJECTION);
		glLoadMatrixf(my_projMatrix);
		return true;

	case UT_MATRIX_VIEW:
		utMatrix4Copy(my_viewMatrix, matrix);
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(my_viewMatrix);
		glMultMatrixf(my_modelMatrix);
		break;

	case UT_MATRIX_MODEL:
		utMatrix4Copy(my_modelMatrix, matrix);
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(my_viewMatrix);
		glMultMatrixf(my_modelMatrix);
		break;
	}

	return true;
}
