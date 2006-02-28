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
		glXDestroyContext(display, context);
		XUnmapWindow(display, window);
		XDestroyWindow(display, window);
	}
};

utRenderTarget utxCreateWindowTarget(void* window)
{
	int screen = DefaultScreen(utx_display);

	/* Make sure GLX is available */
	if (!glXQueryExtension(utx_display, NULL, NULL))
	{
		utxLogError("glXQueryExtension");
		return NULL;
	}

	/* Hardcode a pixel format for now...I'll come back to this */
	int surface[] = { GLX_RGBA, GLX_DEPTH_SIZE, 1, GLX_DOUBLEBUFFER, None };
	XVisualInfo* vi = glXChooseVisual(utx_display, screen, surface);
	if (vi == NULL)
	{
		utxLogError("glXChooseVisual");
		return NULL;
	}

	/* Create the rendering context */
	GLXContext context = glXCreateContext(utx_display, vi, None, GL_TRUE);
	if (context == NULL)
	{
		utxLogError("glXCreateContext");
		return NULL;
	}

	/* Get some information about the parent window */
	Window parent = (Window)window;
	XWindowAttributes xwa;
	XGetWindowAttributes(utx_display, parent, &xwa);

	/* Create a child window to hold the graphics. This approach helps
	 * avoid conflicts with the parent window settings, which may come
	 * from an external toolkit */
	XSetWindowAttributes attributes;
	Window child = XCreateWindow(utx_display, parent,
	                       0, 0, xwa.width, xwa.height, 0,
	                       CopyFromParent, CopyFromParent, 
						   vi->visual, None, &attributes);
	if (!child)
	{
		utxLogError("XCreateWindow");
		return NULL;
	}

	/* Activate the new rendering window */
	XMapRaised(utx_display, child);
	glXMakeCurrent(utx_display, child, context);

	/* All set */
	utxX11RenderTarget* target = utNEW utxX11RenderTarget;
	target->display = utx_display;
	target->window = child;
	target->context = context;
	target->width =  xwa.width;
	target->height = xwa.height;
	return target;
}


int utResizeRenderTarget(utRenderTarget target, int width, int height)
{
	/* Resize the rendering window */
	utxX11RenderTarget* x11t = (utxX11RenderTarget*)target;
	XResizeWindow(x11t->display, x11t->window, width, height);

	/* Apply the change */
	x11t->width = width;
	x11t->height = height;
	glViewport(0, 0, width, height);
	return true;
}


int utSwapRenderTarget(utRenderTarget target)
{
	utxX11RenderTarget* x11t = (utxX11RenderTarget*)target;
	glXSwapBuffers(x11t->display, x11t->window);
	return true;
}
