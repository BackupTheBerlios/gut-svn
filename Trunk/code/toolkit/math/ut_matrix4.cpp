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


void utMatrix4Multiply(float* result, const float* m0, const float* m1)
{
	for (int i = 0; i < 4; ++i)
	{
		result[_(i,0)] = m0[_(0,0)] * m1[_(i,0)] + 
		                 m0[_(1,0)] * m1[_(i,1)] +
							  m0[_(2,0)] * m1[_(i,2)] +
							  m0[_(3,0)] * m1[_(i,3)];

		result[_(i,1)] = m0[_(0,1)] * m1[_(i,0)] + 
		                 m0[_(1,1)] * m1[_(i,1)] +
							  m0[_(2,1)] * m1[_(i,2)] +
							  m0[_(3,1)] * m1[_(i,3)];

		result[_(i,2)] = m0[_(0,2)] * m1[_(i,0)] + 
		                 m0[_(1,2)] * m1[_(i,1)] +
							  m0[_(2,2)] * m1[_(i,2)] +
							  m0[_(3,2)] * m1[_(i,3)];

		result[_(i,3)] = m0[_(0,3)] * m1[_(i,0)] + 
		                 m0[_(1,3)] * m1[_(i,1)] +
							  m0[_(2,3)] * m1[_(i,2)] +
							  m0[_(3,3)] * m1[_(i,3)];
	}
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


void utMatrix4RotationFromAngleAxis(float* matrix, float angle, float x, float y, float z)
{
	float s = sinf(angle);
	float c = cosf(angle);
	float t = 1.0f - c;

	matrix[_(0,0)] = t * x * x + c;
	matrix[_(0,1)] = t * y * x - s * z;
	matrix[_(0,2)] = t * z * x + s * y;
	matrix[_(0,3)] = 0.0f;
	matrix[_(1,0)] = t * x * y + s * z;
	matrix[_(1,1)] = t * y * y + c;
	matrix[_(1,2)] = t * z * y - s * x;
	matrix[_(1,3)] = 0.0f;
	matrix[_(2,0)] = t * x * z - s * y;
	matrix[_(2,1)] = t * y * z + s * x;
	matrix[_(2,2)] = t * z * z + c;
	matrix[_(2,3)] = 0.0f;
	matrix[_(3,0)] = 0.0f;
	matrix[_(3,1)] = 0.0f;
	matrix[_(3,2)] = 0.0f;
	matrix[_(3,3)] = 1.0f;
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
