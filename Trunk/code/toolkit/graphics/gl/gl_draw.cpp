/**********************************************************************
 * GameGut - graphics/gl/gl_draw.cpp
 * Copyright (c) 1999-2006 Jason Perkins.
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


int utDrawLines(const float* vertices, int count)
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 3 * sizeof(float), vertices);
	glDrawArrays(GL_LINES, 0, count);
	glDisableClientState(GL_VERTEX_ARRAY);
	return true;
}


int utDrawPoints(const float* vertices, int count)
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 3 * sizeof(float), vertices);
	glDrawArrays(GL_POINTS, 0, count);
	glDisableClientState(GL_VERTEX_ARRAY);
	return true;
}
