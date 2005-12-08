/**********************************************************************
 * GameGut - gl_x11_rendertarget.cpp
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
#include "core/x11/errors.h"
#include "../gl_graphics.h"
#include <stdio.h>
#include <X11/Xlib.h>
#include <GL/glx.h>


struct utxX11RenderTarget : utxRenderTarget
{
	Display* display;
	Window window;
	GLXContext context;
	
	~utxX11RenderTarget()
	{
		puts("Warning: ~utxX11RenderTarget() not implemented");
	}
};


utRenderTarget utxCreateWindowTarget(void* window)
{
	Display* display = XOpenDisplay(NULL);
	if (display == NULL)
	{
		utxLogError("XOpenDisplay");
		return NULL;
	}

	/* Get some information about the target window */
	Window x11w = (Window)window;
	XWindowAttributes xwa;
	XGetWindowAttributes(display, x11w, &xwa);
	
	int screen = DefaultScreen(display);

	/* Hardcode a pixel format for now...I'll come back to this */
	int surface[] = { GLX_RGBA, GLX_DEPTH_SIZE, 1, GLX_DOUBLEBUFFER, None };
	XVisualInfo* visual = glXChooseVisual(display, screen, surface);
	if (visual == NULL)
	{
		utxLogError("glXChooseVisual");
		return NULL;
	}

	/* Create the rendering context */
	GLXContext context = glXCreateContext(display, visual, NULL, True);
	if (context == NULL)
	{
		utxLogError("glXCreateContext");
		return NULL;
	}

	/* All set */
	utxX11RenderTarget* target = utNEW utxX11RenderTarget;
	target->display = display;
	target->window = (Window)window;
	target->context = context;
	target->width = xwa.width;
	target->height = xwa.height;
	return target;
}


int utResizeRenderTarget(utRenderTarget target, int width, int height)
{
	return true;
}


int utSwapRenderTarget(utRenderTarget target)
{
	utxX11RenderTarget* x11t = (utxX11RenderTarget*)target;
	glXSwapBuffers(x11t->display, x11t->window);
	return true;
}
