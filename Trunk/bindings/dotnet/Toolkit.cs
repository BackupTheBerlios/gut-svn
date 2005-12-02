#region BSD License
/* Sim8.GameGuts - Toolkit.cs
 * Copyright (c) 2001-2005 Jason Perkins.
 * All rights reserved.
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the BSD-style license that is 
 * included with this library in the file LICENSE.txt.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
 * files LICENSE.txt for more details. */
#endregion

using System;
using System.Diagnostics;
using System.Runtime.InteropServices;

namespace Sim8.GameGuts
{
	[CLSCompliant(false)]
	public class Toolkit
	{
		#region Initialization Interface
		[DllImport("GameGuts")] public static extern 
		bool utInitialize();

		[DllImport("GameGuts")] public static extern 
		bool utShutdown();
		#endregion

		#region Logging Interface

		/* Connect Toolkit logging interface to .NET tracing system */
		public delegate void utLogHandler(string message);
		private static utLogHandler _loggingCallback;
		private static bool _loggingEnabled;

		static Toolkit()
		{
			_loggingCallback = new utLogHandler(LoggingCallback);
			Toolkit.utSetLogHandler(_loggingCallback);
			_loggingEnabled = true;
		}

		private static void LoggingCallback(string message)
		{
			if (_loggingEnabled)
				Trace.Write(message, "Toolkit");
		}

		public static bool LoggingEnabled
		{
			get { return _loggingEnabled; }
			set { _loggingEnabled = value; }
		}

		[DllImport("GameGuts")] public static extern 
		void utSetLogHandler(utLogHandler callback);		
		
		#endregion

		#region Memory Interface

		[DllImport("GameGuts")] public static extern
		IntPtr utAlloc(int size, string file, int line);

		[DllImport("GameGuts")] public static extern
		void utFree(IntPtr ptr, string file, int line);

		[DllImport("GameGuts")] public static extern 
		int utGetAllocCount();

		[DllImport("GameGuts")] public static extern 
		int utGetFreeCount();
		
		#endregion

		#region Event Processing Interface

		public enum utEventKind
		{
			UT_EVENT_NONE,
			UT_EVENT_WINDOW_CLOSE,
			UT_EVENT_WINDOW_REDRAW,
			UT_EVENT_WINDOW_RESIZE,
			UT_EVENT_WINDOW_FOCUS,
			UT_EVENT_KEY,
			UT_EVENT_KEY_REPEAT,
			UT_EVENT_CHAR,
			UT_EVENT_MOUSE_AXIS,
			UT_EVENT_MOUSE_BUTTON,
			UT_EVENT_CTRL_AXIS,
			UT_EVENT_CTRL_BUTTON
		}

		[StructLayout(LayoutKind.Sequential)]
		public struct utEvent
		{
			public utEventKind what;
			public IntPtr window;
			public uint   when;
			public int    arg0;
			public int    arg1;
			public int    arg2;
		}

		public delegate void utEventHandler(ref utEvent e);

		[DllImport("GameGuts")] public static extern
		bool utPollEvents(bool block);

		[DllImport("GameGuts")] public static extern
		void utSetEventHandler(utEventHandler callback);
		#endregion

		#region Windowing Interface

		[DllImport("GameGuts")] public static extern
		IntPtr utCreateWindow(string title, int width, int height);

		[DllImport("GameGuts")] public static extern
		bool utDestroyWindow(IntPtr window);

		[DllImport("GameGuts")] public static extern
		IntPtr utGetWindowHandle(IntPtr window);
			
		#endregion

		#region Timer Interface

		[DllImport("GameGuts")] public static extern
		int utGetTimer();

		#endregion

		#region Graphics Interface

		[DllImport("GameGuts")] public static extern
		bool utBeginFrame();
	
		[DllImport("GameGuts")] public static extern
		bool utClear(float red, float green, float blue, float alpha);

		[DllImport("GameGuts")] public static extern
		IntPtr utCreateWindowTarget(IntPtr parentWindow);

		[DllImport("GameGuts")] public static extern
		bool utEndFrame();

		[DllImport("GameGuts")] public static extern
		bool utReleaseRenderTarget(IntPtr target);

		[DllImport("GameGuts")] public static extern
		bool utResizeRenderTarget(IntPtr target, int width, int height);
		
		[DllImport("GameGuts")] public static extern
		bool utSwapAllRenderTargets();

		#endregion
	}
}
