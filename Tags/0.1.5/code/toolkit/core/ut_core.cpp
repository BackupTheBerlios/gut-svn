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


int utInitialize()
{
	utLog("GameGuts Toolkit version " GUT_VERSION "\n");
	utLog("Beginning Toolkit initialization\n");

	if (!utxInitializeMemory())    return false;
	if (!utxInitializePlatform())  return false;

	utLog("Toolkit initialization complete\n");
	return true;
}


int utShutdown()
{
	utLog("Shutting down GameGuts Toolkit\n");

	bool result = true;
	result = result && utxShutdownPlatform();
	result = result && utxShutdownMemory();
	return result;
}
