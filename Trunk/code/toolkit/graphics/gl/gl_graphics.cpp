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
	utxReleaseAllTextures();
	utxReleaseAllVertexBuffers();
	utxReleaseAllVertexFormats();
	utxReleaseAllRenderTargets();
	return true;
}


int utBeginFrame()
{
	/* Initialize matrices to something reasonable */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	/* Set up a useful default state. This will all get phased out once
	 * I've gotten the the shader code sorted */
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_POINT_SMOOTH);

	glCullFace(GL_CCW);
	glPointSize(5.0f);

	GLfloat position [] = { 1.0, 1.0, 1.0, 0.0 };
	GLfloat specular [] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat shininess [] = { 100.0 };

	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
	
	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	return true;
}


int utClear(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	return true;
}


int utDraw(utVertexBuffer vertices, utVertexFormat format, utIndexBuffer indices, int start, int count)
{
	float* ptr = vertices->data;
	for (int i = 0; i < format->count; ++i)
	{
		switch (format->attributes[i])
		{
		case UT_VERTEX_POSITION:
			glEnableClientState(GL_VERTEX_ARRAY);
			glVertexPointer(3, GL_FLOAT, format->stride, ptr);
			ptr += 3;
			break;
		case UT_VERTEX_NORMAL:
			glEnableClientState(GL_NORMAL_ARRAY);
			glNormalPointer(GL_FLOAT, format->stride, ptr);
			ptr += 3;
			break;
		case UT_VERTEX_TEXTURE2:
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glTexCoordPointer(2, GL_FLOAT, format->stride, ptr);
			ptr += 2;
			break;
		}
	}

	if (count == UT_DRAW_ALL)
		count = indices->size;

	glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, indices->data);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	return true;
}


int utEndFrame()
{
	return true;
}


int utGrabScreen(utRenderTarget target, void* buffer)
{
	glReadBuffer(GL_FRONT);
	glReadPixels(0, 0, target->width, target->height, GL_BGR, GL_UNSIGNED_BYTE, buffer);
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


int utSetTexture(int stage, utTexture texture)
{
	if (texture != NULL)
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture->handle);
	}
	else
	{
		glDisable(GL_TEXTURE_2D);
	}
	return true;
}
