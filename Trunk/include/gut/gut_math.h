/**********************************************************************
 * GameGut - gut_math.h
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

UT_EXPORT void utMatrix4Copy(float* dest, const float* src);
UT_EXPORT void utMatrix4Multiply(float* result, const float* m0, const float* m1);
UT_EXPORT void utMatrix4Perspective(float* matrix, float fieldOfView, float aspectRatio, float nearClip, float farClip);
UT_EXPORT void utMatrix4RotationFromAngleAxis(float* matrix, float angle, float x, float y, float z);
UT_EXPORT void utMatrix4SetIdentity(float* matrix);
UT_EXPORT void utMatrix4Translation(float* matrix, float x, float y, float z);
