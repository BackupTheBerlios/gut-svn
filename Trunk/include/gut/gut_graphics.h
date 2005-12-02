/**********************************************************************
 * GameGut - gut_graphics.h
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

struct utxRenderTarget;
typedef struct utxRenderTarget* utRenderTarget;


UT_EXPORT int            utBeginFrame();
UT_EXPORT int            utClear(float r, float g, float b, float a);
UT_EXPORT utRenderTarget utCreateWindowTarget(void* window);
UT_EXPORT int            utEndFrame();
UT_EXPORT int            utReleaseRenderTarget(utRenderTarget target);
UT_EXPORT int            utResizeRenderTarget(utRenderTarget target, int width, int height);
UT_EXPORT int            utSwapRenderTarget(utRenderTarget target);
UT_EXPORT int            utSwapAllRenderTargets();
