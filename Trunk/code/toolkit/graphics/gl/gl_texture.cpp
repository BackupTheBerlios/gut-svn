/**********************************************************************
 * GameGut - graphics/gl/gl_texture.cpp
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
#include "core/core.h"
#include "gl_graphics.h"

/* This is _real_ barebones right now; I'm just trying to get something
 * on the screen quickly so I can bring the physics online */

/* Keep track of all open buffers so I can close them at exit */
static utxArray<utTexture> my_textures;


utTexture utCreateTexture(int width, int height, utTextureFormat format, const void* data)
{
	GLenum glFormat;
	GLenum glInternalFormat;
	switch (format)
	{
	case UT_TEXTURE_RGB8:
		glFormat = GL_RGB;
		glInternalFormat = GL_RGB8;
		break;
	case UT_TEXTURE_RGBA8:
		glFormat = GL_RGBA;
		glInternalFormat = GL_RGBA8;
		break;
	default:
		utLog("utCreateTexture: invalid format\n");
		return NULL;
	}

	GLuint handle;
	glGenTextures(1, &handle);
	glBindTexture(GL_TEXTURE_2D, handle);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	gluBuild2DMipmaps(GL_TEXTURE_2D, glInternalFormat, width, height, glFormat, GL_UNSIGNED_BYTE, data);

	int error = glGetError();
	if (error != 0)
	{
		glDeleteTextures(1, &handle);
		char msg[128];
		sprintf(msg, "gluBuild2DMipmaps() failed with error %d\n", error);
		utLog(msg);
		return NULL;
	}

	utTexture texture = utNEW utxTexture();
	texture->handle = handle;

	/* Add the buffer to the master list */
	texture->reference();
	my_textures.push_back(texture);
	return texture;
}


utxTexture::~utxTexture()
{
	glDeleteTextures(1, &handle);
}


void utxReleaseAllTextures()
{
	for (int i = 0; i < my_textures.size(); ++i)
		my_textures[i]->release();
	my_textures.clear();
}	

