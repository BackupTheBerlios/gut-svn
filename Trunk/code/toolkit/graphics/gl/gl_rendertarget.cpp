/**********************************************************************
 * GameGut - gl_rendertarget.cpp
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
#include "graphics/graphics.h"

/* Temporary, I'll need to create a platform-dependent class */
struct utxRenderTarget : Referenced
{
	int x;

	utxRenderTarget()
	{
		x = 22;
	}

	~utxRenderTarget()
	{
		x = 0;
	}
};


/* Keep track of all open targets so I can close them at exit */
static utxArray<utRenderTarget> my_targets;


utRenderTarget utCreateWindowTarget(void* window)
{
	utRenderTarget target = new utxRenderTarget();

	target->reference();
	my_targets.push_back(target);
	return target;
}


void utxReleaseAllRenderTargets()
{
	for (int i = 0; i < my_targets.size(); ++i)
		my_targets[i]->release();
	my_targets.clear();
}
