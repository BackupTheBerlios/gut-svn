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


/* Helper to switch between OpenGL and my custom format codes */
typedef struct MyFormatInfo
{
	GLenum internal;
	GLenum external;
	GLenum datatype;
} MyFormatInfo;

static bool MyIdentifyFormat(MyFormatInfo* info, utTextureFormat format);


/* Keep track of all open buffers so I can close them at exit */
static utxArray<utTexture> my_textures;


utTexture utCreateTexture(int width, int height, utTextureFormat format)
{
	MyFormatInfo info;
	if (!MyIdentifyFormat(&info, format))
		return NULL;

	utTexture texture = utNEW utxTexture();
	texture->handle = 0;
	texture->width = width;
	texture->height = height;
	texture->format = format;

	glGenTextures(1, &texture->handle);
	glBindTexture(GL_TEXTURE_2D, texture->handle);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	/* I haven't implemented mipmaps yet */
	int mipLevel = 0;

	/* Build placeholder texture, to be replaced in CopyData() below */
	glTexImage2D(GL_TEXTURE_2D, mipLevel, info.internal, width, height, 0, info.external, info.datatype, NULL);
	if (utxTestGLError("glTexImage2D"))
	{
		utDELETE texture;
		return NULL;
	}

	/* Add the buffer to the master list */
	texture->reference();
	my_textures.push_back(texture);
	return texture;
}


utxTexture::~utxTexture()
{
	glDeleteTextures(1, &handle);
}


int utReleaseTexture(utTexture texture)
{
	if (my_textures.remove(texture))
		texture->release();
	return true;
}


void utxReleaseAllTextures()
{
	for (int i = 0; i < my_textures.size(); ++i)
		my_textures[i]->release();
	my_textures.clear();
}	


int utCopyTextureData(utTexture texture, const void* data)
{
	MyFormatInfo info;
	MyIdentifyFormat(&info, texture->format);

	glBindTexture(GL_TEXTURE_2D, texture->handle);

	/* I haven't implemented mipmaps yet */
	int mipLevel = 0;

	/* I'll eventually need a way to upload just a portion of the texture */
	int left = 0;
	int top  = 0;
	int width = texture->width;
	int height = texture->height;

	/* I'll also need to support format conversions */
	int dataFormat = info.external;
	int dataType   = info.datatype;

	/* Upload the new texture data */
   glTexSubImage2D(GL_TEXTURE_2D, mipLevel, left, top, width, height, dataFormat, dataType, data);
	if (utxTestGLError("glTexSubImage2D"))
		return false;

	return true;
}


/****************************************************************************
 * Convert between my custom texture format codes and the OpenGL versions.
 ****************************************************************************/

static bool MyIdentifyFormat(MyFormatInfo* info, utTextureFormat format)
{
	switch (format)
	{
	case UT_TEXTURE_R8G8B8:
		info->internal = GL_RGB8;
		info->external = GL_RGB;
		info->datatype = GL_UNSIGNED_BYTE;
		break;
	case UT_TEXTURE_R8G8B8A8:
		info->internal = GL_RGBA8;
		info->external = GL_RGBA;
		info->datatype = GL_UNSIGNED_BYTE;
		break;
	default:
		char msg[128];
		sprintf(msg, "Invalid texture format: %d\n", format);
		utLog(msg);
		return false;
	}
	return true;
}


