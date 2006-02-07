/**********************************************************************
 * GameGut - ut_core.cpp
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
#include "platform/platform.h"
#include "graphics/graphics.h"

static int my_initCount = 0;

int utInitialize()
{
	if (my_initCount == 0)
	{
		my_initCount++;

		utLog("GameGuts Toolkit version " GUT_VERSION "\n");
		utLog("Beginning Toolkit initialization\n");

		if (!utxInitializeMemory())    return false;
#if !defined(NO_PLATFORM)
		if (!utxInitializePlatform())  return false;
#endif
#if !defined(NO_GRAPHICS)
		if (!utxInitializeGraphics())  return false;
#endif

		utLog("Toolkit initialization complete\n");
	}
	return true;
}


int utShutdown()
{
	/* If I'm already shutdown, just return */
	if (my_initCount == 0)
		return true;

	my_initCount--;
	if (my_initCount == 0)
	{
		utLog("Shutting down GameGuts Toolkit\n");

		bool result = true;
#if !defined(NO_GRAPHICS)
		result = result && utxShutdownGraphics();
#endif
#if !defined(NO_PLATFORM)
		result = result && utxShutdownPlatform();
#endif
		result = result && utxShutdownMemory();
		return result;
	}

	return true;
}
