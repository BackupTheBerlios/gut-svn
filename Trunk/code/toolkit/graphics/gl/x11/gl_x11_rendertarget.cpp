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
#include "platform/x11/x11_platform.h"  /* for common display pointer */
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
#if HOLDING
		glXDestroyContext(display, context);
#endif
	}
};

utRenderTarget utxCreateWindowTarget(void* window)
{
	int screen = DefaultScreen(utx_display);

	XSetWindowAttributes attributes;
	attributes.background_pixel = WhitePixel(utx_display, screen);
	attributes.border_pixel = 0;
	attributes.colormap = XDefaultColormap(utx_display, screen);

	Window gfxWnd = XCreateWindow(utx_display, (Window)window,
	                       10, 10, 100, 100, 
						   0,
	                       CopyFromParent, 
						   CopyFromParent, 
						   CopyFromParent, /* visual */
	                       CWBackPixel | CWBorderPixel | CWColormap, 
	                       &attributes);
	if (!gfxWnd)
	{
		utxLogError("XCreateWindow");
		return NULL;
	}

	XMapRaised(utx_display, gfxWnd);

#if HOLDING
	/* Make sure GLX is available */
	if (!glXQueryExtension(display, NULL, NULL))
	{
		utxLogError("glXQueryExtension");
		return NULL;
	}

	/* Get some information about the target window */
	Window x11window = (Window)window;
	XWindowAttributes xwa;
	XGetWindowAttributes(display, x11window, &xwa);
	
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

	glXMakeCurrent(display, x11window, context);
#endif

	/* All set */
	utxX11RenderTarget* target = utNEW utxX11RenderTarget;
	target->display = utx_display;
	target->window = gfxWnd;
#if HOLDING
	target->context = context;
	target->width =  xwa.width;
	target->height = xwa.height;
#endif
	return target;
}


int utResizeRenderTarget(utRenderTarget target, int width, int height)
{
#if HOLDING
	target->width = width;
	target->height = height;
	glViewport(0, 0, width, height);
#endif
	return true;
}


int utSwapRenderTarget(utRenderTarget target)
{
#if HOLDING
	utxX11RenderTarget* x11t = (utxX11RenderTarget*)target;
	glXSwapBuffers(x11t->display, x11t->window);
#endif
	return true;
}
