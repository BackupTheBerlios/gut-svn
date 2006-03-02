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

enum utBufferFlags
{
	UT_BUFFER_NONE
};

enum utRenderMatrix
{
	UT_MATRIX_PROJECTION,
	UT_MATRIX_VIEW,
	UT_MATRIX_MODEL
};

enum utTextureFormat
{
	UT_TEXTURE_R8G8B8,
	UT_TEXTURE_R8G8B8A8
};

enum utVertexAttribute
{
	UT_VERTEX_POSITION,
	UT_VERTEX_NORMAL,
	UT_VERTEX_TEXTURE2
};


#define UT_DRAW_ALL   (-1)


struct utxIndexBuffer;
struct utxRenderTarget;
struct utxTexture;
struct utxVertexBuffer;
struct utxVertexFormat;

typedef struct utxIndexBuffer*  utIndexBuffer;
typedef struct utxRenderTarget* utRenderTarget;
typedef struct utxTexture*      utTexture;
typedef struct utxVertexBuffer* utVertexBuffer;
typedef struct utxVertexFormat* utVertexFormat;


UT_EXPORT int            utBeginFrame();
UT_EXPORT int            utClear(float r, float g, float b, float a);
UT_EXPORT int            utCopyIndexData(utIndexBuffer buffer, const int* data, int size);
UT_EXPORT int            utCopyTextureData(utTexture texture, const void* data);
UT_EXPORT int            utCopyVertexData(utVertexBuffer buffer, const float* data, int size);
UT_EXPORT utIndexBuffer  utCreateIndexBuffer(int size, utBufferFlags flags);
UT_EXPORT utTexture      utCreateTexture(int width, int height, utTextureFormat format);
UT_EXPORT utVertexBuffer utCreateVertexBuffer(int size, utBufferFlags flags);
UT_EXPORT utVertexFormat utCreateVertexFormat(const utVertexAttribute* attributes, int count);
UT_EXPORT utRenderTarget utCreateWindowTarget(void* display, void* window);
UT_EXPORT int            utDraw(utVertexBuffer vertices, utVertexFormat format, utIndexBuffer indices, int start, int count);
UT_EXPORT int            utEndFrame();
UT_EXPORT int            utGetTargetHeight(utRenderTarget target);
UT_EXPORT int            utGetTargetSizeInBytes(utRenderTarget target);
UT_EXPORT int            utGetTargetWidth(utRenderTarget target);
UT_EXPORT int            utGrabScreen(utRenderTarget target, void* buffer);
UT_EXPORT int            utReleaseIndexBuffer(utIndexBuffer buffer);
UT_EXPORT int            utReleaseRenderTarget(utRenderTarget target);
UT_EXPORT int            utReleaseTexture(utTexture texture);
UT_EXPORT int            utReleaseVertexBuffer(utVertexBuffer buffer);
UT_EXPORT int            utReleaseVertexFormat(utVertexFormat format);
UT_EXPORT int            utResizeRenderTarget(utRenderTarget target, int width, int height);
UT_EXPORT int            utSetRenderMatrix(utRenderMatrix which, const float* matrix);
UT_EXPORT int            utSetTexture(int stage, utTexture texture);
UT_EXPORT int            utSwapRenderTarget(utRenderTarget target);
UT_EXPORT int            utSwapAllRenderTargets();
