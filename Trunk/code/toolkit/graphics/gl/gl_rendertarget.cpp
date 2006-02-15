/**********************************************************************
 * GameGut - graphics/gl/gl_rendertarget.cpp
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

/* Keep track of all open targets so I can close them at exit */
static utxArray<utRenderTarget> my_targets;


utRenderTarget utCreateWindowTarget(void* window)
{
	utRenderTarget target = utxCreateWindowTarget(window);
	if (target == NULL)
		return NULL;
	
	/* Add it to the master list */
	target->reference();
	my_targets.push_back(target);
	return target;
}


int utGetTargetHeight(utRenderTarget target)
{
	return target->height;
}


int utGetTargetSizeInBytes(utRenderTarget target)
{
	/* My pixel format is hardcoded right now */
	return (target->width * target->height * 3);
}


int utGetTargetWidth(utRenderTarget target)
{
	return target->width;
}


int utReleaseRenderTarget(utRenderTarget target)
{
	my_targets.remove(target);
	target->release();
	return true;
}


void utxReleaseAllRenderTargets()
{
	for (int i = 0; i < my_targets.size(); ++i)
		my_targets[i]->release();
	my_targets.clear();
}


int utSwapAllRenderTargets()
{
	int result = true;
	for (int i = 0; i < my_targets.size(); ++i)
	{
		if (!utSwapRenderTarget(my_targets[i]))
			result = false;
	}
	return result;
}


