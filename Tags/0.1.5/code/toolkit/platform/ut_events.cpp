/**********************************************************************
 * GameGut - ut_events.cpp
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

static utEventHandler my_handler = NULL;


void utSetEventHandler(utEventHandler callback)
{
	my_handler = callback;
}


int utPollEvents(int block)
{
	if (utNumWindows() == 0)
		return false;

	return utxPollEvents(block);
}


int utSendEvent(utEvent* event)
{
	if (my_handler == NULL)
		return false;

	my_handler(event);
	return true;
}
