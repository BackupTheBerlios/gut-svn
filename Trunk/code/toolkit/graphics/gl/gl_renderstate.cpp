/**********************************************************************
 * GameGut - graphics/gl/gl_renderstate.cpp
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


int utSetRenderState(utRenderState state, unsigned int value)
{
	switch (state)
	{
	case UT_RS_LIGHTING:
		value ? glEnable(GL_LIGHTING) : glDisable(GL_LIGHTING);
		return true;
	}

	char msg[128];
	sprintf_s(msg, 128, "Unknown render state %d\n", state);
	utLog(msg);
	return false;
}
