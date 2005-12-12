/**********************************************************************
 * GameGut - graphics/gl/gl_vertexformat.cpp
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


/* Keep track of all open formats so I can close them at exit */
static utxArray<utVertexFormat> my_formats;


utVertexFormat utCreateVertexFormat(const utVertexAttribute* attributes, int count)
{
	utVertexFormat format = utNEW utxVertexFormat();
	format->attributes = (utVertexAttribute*)utALLOC(sizeof(utVertexAttribute) * count);
	format->count = count;
	format->stride = 0;

	for (int i = 0; i < count; ++i)
	{
		format->attributes[i] = attributes[i];
		switch (attributes[i])
		{
		case UT_VERTEX_POSITION:
			format->stride += 3 * sizeof(float);
			break;
		case UT_VERTEX_NORMAL:
			format->stride += 3 * sizeof(float);
			break;
		case UT_VERTEX_TEXTURE2:
			format->stride += 2 * sizeof(float);
			break;
		default:
			utDELETE format;
			utLog("utCreateVertexFormat: invalid attribute\n");
			return NULL;
		}
	}

	/* Add the buffer to the master list */
	format->reference();
	my_formats.push_back(format);
	return format;
}


utxVertexFormat::~utxVertexFormat()
{
	utFREE(attributes);
}


int utReleaseVertexFormat(utVertexFormat format)
{
	if (my_formats.remove(format))
		format->release();
	return true;
}


void utxReleaseAllVertexFormats()
{
	for (int i = 0; i < my_formats.size(); ++i)
		my_formats[i]->release();
	my_formats.clear();
}	

