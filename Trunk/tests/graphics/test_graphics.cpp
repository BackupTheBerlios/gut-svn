/**********************************************************************
 * GameGut - test_graphics.cpp
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
#include <stdlib.h>
#include <gut/gut.h>


/* Some simple cube data for rendering */
float vertices[] = 
{
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f
};

utVertexAttribute format[] =
{
	UT_VERTEX_POSITION,
	UT_VERTEX_NORMAL,
	UT_VERTEX_TEXTURE2
};

int indices[] = 
{
	0, 1, 2,
	0, 2, 3,
	4, 5, 6,
	4, 6, 7,
	8, 9, 10,
	8, 10, 11,
	12, 13, 14,
	12, 14, 15,
	16, 17, 18,
	16, 18, 19,
	20, 21, 22,
	20, 22, 23
};

int texture[] =
{
	0xff000000, 0xff000000, 0xffffffff, 0xffffffff, 
	0xff000000, 0xff000000, 0xffffffff, 0xffffffff, 
	0xffffffff, 0xffffffff, 0xff000000, 0xff000000, 
	0xffffffff, 0xffffffff, 0xff000000, 0xff000000
};


/* My rendering objects */
static utRenderTarget target;
static utIndexBuffer ibuf;
static utVertexBuffer vbuf;
static utVertexFormat vfmt;
static utTexture tex;

static bool keepRunning;
static int  startTime;
static int  numFrames;


void UT_CALLBACK onEvent(utEvent* event)
{
	switch (event->what)
	{
	case UT_EVENT_KEY:
		if (event->arg1 == UT_KEY_ESCAPE && event->arg2 > 0)
			keepRunning = false;
		break;

	case UT_EVENT_WINDOW_CLOSE:
		utDestroyWindow(event->window);
		break;

	case UT_EVENT_WINDOW_RESIZE:
		utResizeRenderTarget(target, event->arg0, event->arg1);
		break;
	}
}


void UT_CALLBACK onLogMessage(const char* message)
{
	printf(message);
}


void die(const char* msg)
{
	puts(msg);
	exit(1);
}


void tick()
{
	float elapsed = (utGetTimer() - startTime) / 1000.0f;
	numFrames++;

	utBeginFrame();
	utClear(0.2f, 0.0f, 0.2f, 1.0f);

	/* Set up a 3D projection */
	float matrix[16];
	utMatrix4Perspective(matrix, 1.0f, (640.0f / 480.0f), 0.1f, 100.0f);
	utSetRenderMatrix(UT_MATRIX_PROJECTION, matrix);

	/* Push the model back a bit */
	utMatrix4Translation(matrix, 0.0f, 0.0f, -2.5f);
	utSetRenderMatrix(UT_MATRIX_VIEW, matrix);

	/* Spin it slowly */
	utMatrix4RotationFromAngleAxis(matrix, elapsed, 0.8944f, 0.4472f, 0.0f);
	utSetRenderMatrix(UT_MATRIX_MODEL, matrix);

	utSetTexture(0, tex);
	utDraw(vbuf, vfmt, ibuf, 0, UT_DRAW_ALL);

	utEndFrame();
	utSwapRenderTarget(target);
}


int main()
{
	utSetLogHandler(onLogMessage);
	utInitialize();

	/* Create a window */
	utWindow wnd = utCreateWindow("Toolkit Graphics Test", 640, 480);
	if (wnd == NULL)
		die("Failed to create window");

	/* Attach a render target to the window */
	void* hwnd = utGetWindowHandle(wnd);
	target = utCreateWindowTarget(hwnd);

	/* Build some geometry for rendering */
	int count = sizeof(vertices) / sizeof(float); 
	vbuf = utCreateVertexBuffer(count, UT_BUFFER_NONE);
	if (vbuf == NULL)
		die("Unable to create vertex buffer!");
	if (!utCopyVertexData(vbuf, vertices, count))
		die("Unable to copy vertices!");

	count = sizeof(format) / sizeof(utVertexAttribute);
	vfmt = utCreateVertexFormat(format, count);
	if (vfmt == NULL)
		die("Unable to create vertex format!");

	count = sizeof(indices) / sizeof(int);
	ibuf = utCreateIndexBuffer(count, UT_BUFFER_NONE);
	if (ibuf == NULL)
		die("Unable to create index buffer!");
	if (!utCopyIndexData(ibuf, indices, count))
		die("Unable to copy indices!");

	tex = utCreateTexture(4, 4, UT_TEXTURE_RGBA8, texture);
	if (tex == NULL)
		die("Unable to create texture!");

	/* Run the event loop */
	startTime = utGetTimer();
	numFrames = 0;

	utSetEventHandler(onEvent);
	keepRunning = true;
	while (utPollEvents(false) && keepRunning)
	{
		tick();
	}

	char result[128];
	float elapsed = (utGetTimer() - startTime) / 1000.0f;
	sprintf(result, "FPS: %f\n", numFrames / elapsed);
	utLog(result);

	/* Clean up */
	utShutdown();
	return 0;
}
