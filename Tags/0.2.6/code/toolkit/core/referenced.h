/**********************************************************************
 * GameGut - referenced.h
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


/**********************************************************************
 * A reference-counted base class and matching smart pointer template
 *********************************************************************/

struct Referenced
{
	Referenced()
	{ ref = 0; }

	virtual ~Referenced()
	{ }

	void reference()
	{ ++ref; }

	void release()
	{ if (--ref <= 0) utDELETE this; }

	int ref;
	
	static void* operator new(size_t size, const char* file, int line)
	{
		return utAlloc(size, file, line);;
	}
	
	static void operator delete(void* ptr, const char* file, int line)
	{
		return utFree(ptr, file, line);
	}

	static void operator delete(void* ptr)
	{
		utFree(ptr, NULL, 0);
	} 
};


template<class T>
class Ref
{
public:
	Ref() : m_ptr(0)                       { }
	Ref(T ptr) : m_ptr(ptr)                { if (m_ptr) ((Referenced*)m_ptr)->reference(); }
	Ref(const Ref& ref) : m_ptr(ref.m_ptr) { if (m_ptr) ((Referenced*)m_ptr)->reference(); }
	
	~Ref()                           { if (m_ptr) ((Referenced*)m_ptr)->release(); }

	T ptr()                          { return m_ptr; }
	T operator ->()                  { return m_ptr; }
	const T operator ->() const      { return m_ptr; }
	operator T()                     { return m_ptr; }
	operator const T() const         { return m_ptr; }

	Ref& operator =(const Ref& ref)
	{
		if (ref.m_ptr) ((Referenced*)ref.m_ptr)->reference();
		if (m_ptr) ((Referenced*)m_ptr)->release();
		m_ptr = ref.m_ptr;
		return *this;
	}

	Ref& operator =(T ptr)
	{
		if (ptr) ((Referenced*)ptr)->reference();
		if (m_ptr) ((Referenced*)m_ptr)->release();
		m_ptr = ptr;
		return *this;
	}

	bool operator ==(const Ref& ref) const
	{
		return m_ptr == ref.m_ptr;
	}

	bool operator ==(const T ptr) const
	{
		return m_ptr == ptr;
	}

	bool operator !=(const Ref& ref) const
	{
		return (m_ptr != ref.m_ptr);
	}

	bool operator !=(const T ptr) const
	{
		return (m_ptr != ptr);
	}


private:
	T m_ptr;
};
