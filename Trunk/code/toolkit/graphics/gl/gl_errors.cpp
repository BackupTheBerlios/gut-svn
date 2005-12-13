/**********************************************************************
 * GameGut - graphics/gl/gl_errors.cpp
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

int utxTestGLErrorFunc(const char* function, const char* file, int line)
{
	GLenum code = glGetError();
	if (code != GL_NO_ERROR)
	{
		char buffer[8192];
		sprintf(buffer, "%s(%d): call to %s() failed with error code %x: %s\n", file, line, function, code, gluErrorString(code));
		utLog(buffer);
		return true;
	}
	else
	{
		return false;
	}
}

