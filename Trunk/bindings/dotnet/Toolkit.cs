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

namespace GameGuts
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
		
		/* Rudimentary memory tracking - not ready for production use */
		[DllImport("GameGuts")] public static extern 
		void utEnableMemoryDebugging();

		[DllImport("GameGuts")] public static extern 
		void utShowMemoryReport();
			
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
			
		[DllImport("GameGuts")] public static extern
		int utGetWindowHeight(IntPtr window);
	
		[DllImport("GameGuts")] public static extern
		int utGetWindowWidth(IntPtr window);

		[DllImport("GameGuts")] public static extern
		bool utResizeWindow(IntPtr window, int width, int height);
	
		#endregion

		#region Timer Interface

		[DllImport("GameGuts")] public static extern
		int utGetTimer();

		#endregion

		#region Graphics Interface

		public enum utBufferFlags
		{
			UT_BUFFER_NONE
		}

		public enum utRenderMatrix
		{
			UT_MATRIX_PROJECTION,
			UT_MATRIX_VIEW,
			UT_MATRIX_MODEL
		}

		public enum utTextureFormat
		{
			UT_TEXTURE_RGB8,
			UT_TEXTURE_RGBA8
		};

		public enum utVertexAttribute
		{
			UT_VERTEX_POSITION,
			UT_VERTEX_NORMAL,
			UT_VERTEX_TEXTURE2
		}

		[StructLayout(LayoutKind.Sequential)]
		public struct utMatrix4
		{
			public float M00, M10, M20, M30;
			public float M01, M11, M21, M31;
			public float M02, M12, M22, M32;
			public float M03, M13, M23, M33;
		}

		[DllImport("GameGuts")] public static extern
		bool utBeginFrame();
	
		[DllImport("GameGuts")] public static extern
		bool utClear(float red, float green, float blue, float alpha);

		[DllImport("GameGuts")] public static extern
		bool utCopyIndexData(IntPtr buffer, int[] data, int size);

		[DllImport("GameGuts")] public static extern
		bool utCopyTextureData(IntPtr texture, byte[] data);

		[DllImport("GameGuts")] public static extern
		bool utCopyTextureData(IntPtr texture, uint[] data);

		[DllImport("GameGuts")] public static extern
		bool utCopyVertexData(IntPtr buffer, float[] data, int size);

		[DllImport("GameGuts")] public static extern
		IntPtr utCreateIndexBuffer(int size, utBufferFlags flags);

		[DllImport("GameGuts")] public static extern
		IntPtr utCreateTexture(int width, int height, utTextureFormat format);

		[DllImport("GameGuts")] public static extern
		IntPtr utCreateVertexBuffer(int size, utBufferFlags flags);

		[DllImport("GameGuts")] public static extern
		IntPtr utCreateVertexFormat(utVertexAttribute[] attributes, int count);

		[DllImport("GameGuts")] public static extern
		IntPtr utCreateWindowTarget(IntPtr parentWindow);

		[DllImport("GameGuts")] public static extern
		bool utDraw(IntPtr vertices, IntPtr format, IntPtr indices, int start, int count);

		[DllImport("GameGuts")] public static extern
		bool utEndFrame();

		[DllImport("GameGuts")] public static extern
		int utGetTargetHeight(IntPtr target);

		[DllImport("GameGuts")] public static extern
		int utGetTargetSizeInBytes(IntPtr target);

		[DllImport("GameGuts")] public static extern
		int utGetTargetWidth(IntPtr target);

		[DllImport("GameGuts")] public static extern
		bool utGrabScreen(IntPtr target, IntPtr buffer);

		[DllImport("GameGuts")] public static extern
		bool utReleaseIndexBuffer(IntPtr buffer);

		[DllImport("GameGuts")] public static extern
		bool utReleaseRenderTarget(IntPtr target);

		[DllImport("GameGuts")] public static extern
		bool utReleaseTexture(IntPtr texture);

		[DllImport("GameGuts")] public static extern
		bool utReleaseVertexBuffer(IntPtr buffer);

		[DllImport("GameGuts")] public static extern
		bool utReleaseVertexFormat(IntPtr format);

		[DllImport("GameGuts")] public static extern
		bool utResizeRenderTarget(IntPtr target, int width, int height);

		[DllImport("GameGuts")] public static extern
		bool utSetRenderMatrix(utRenderMatrix which, ref float matrix);

		[DllImport("GameGuts")] public static extern
		bool utSetTexture(int stage, IntPtr texture);
		
		[DllImport("GameGuts")] public static extern
		bool utSwapRenderTarget(IntPtr target);

		[DllImport("GameGuts")] public static extern
		bool utSwapAllRenderTargets();

		#endregion
	}
}
