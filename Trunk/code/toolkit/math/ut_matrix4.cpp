/**********************************************************************
 * GameGut - math/ut_matrix4.cpp
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

#include <math.h>
#include "core/core.h"

#define _(r,c)  (c * 4 + r)


void utMatrix4Copy(float* dest, const float* src)
{
	memcpy(dest, src, sizeof(float) * 16);
}


void utMatrix4Perspective(float* matrix, float fieldOfView, float aspectRatio, float nearClip, float farClip)
{
	float ty = fieldOfView * 0.5f;
	float tty = tanf(ty);

	float w = 1.0f / tty / aspectRatio;
	float h = 1.0f / tty;
	float q = -(farClip + nearClip) / (farClip - nearClip);
	float n = -2.0f * (farClip * nearClip) / (farClip - nearClip);

	matrix[_(0,0)] = w;
	matrix[_(0,1)] = 0.0f;
	matrix[_(0,2)] = 0.0f;
	matrix[_(0,3)] = 0.0f;
	matrix[_(1,0)] = 0.0f;
	matrix[_(1,1)] = h;
	matrix[_(1,2)] = 0.0f;
	matrix[_(1,3)] = 0.0f;
	matrix[_(2,0)] = 0.0f;
	matrix[_(2,1)] = 0.0f;
	matrix[_(2,2)] = q;
	matrix[_(2,3)] = n;
	matrix[_(3,0)] = 0.0f;
	matrix[_(3,1)] = 0.0f;
	matrix[_(3,2)] = -1.0f;
	matrix[_(3,3)] = 0.0f;
}


void utMatrix4SetIdentity(float* matrix)
{
	matrix[_(0,0)] = 1.0f;
	matrix[_(0,1)] = 0.0f;
	matrix[_(0,2)] = 0.0f;
	matrix[_(0,3)] = 0.0f;
	matrix[_(1,0)] = 0.0f;
	matrix[_(1,1)] = 1.0f;
	matrix[_(1,2)] = 0.0f;
	matrix[_(1,3)] = 0.0f;
	matrix[_(2,0)] = 0.0f;
	matrix[_(2,1)] = 0.0f;
	matrix[_(2,2)] = 1.0f;
	matrix[_(2,3)] = 0.0f;
	matrix[_(3,0)] = 0.0f;
	matrix[_(3,1)] = 0.0f;
	matrix[_(3,2)] = 0.0f;
	matrix[_(3,3)] = 1.0f;
}


void utMatrix4Translation(float* matrix, float x, float y, float z)
{
	matrix[_(0,0)] = 1.0f;
	matrix[_(0,1)] = 0.0f;
	matrix[_(0,2)] = 0.0f;
	matrix[_(0,3)] = x;
	matrix[_(1,0)] = 0.0f;
	matrix[_(1,1)] = 1.0f;
	matrix[_(1,2)] = 0.0f;
	matrix[_(1,3)] = y;
	matrix[_(2,0)] = 0.0f;
	matrix[_(2,1)] = 0.0f;
	matrix[_(2,2)] = 1.0f;
	matrix[_(2,3)] = z;
	matrix[_(3,0)] = 0.0f;
	matrix[_(3,1)] = 0.0f;
	matrix[_(3,2)] = 0.0f;
	matrix[_(3,3)] = 1.0f;
}
