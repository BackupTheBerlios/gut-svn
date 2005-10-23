package.name = "GameGuts"
package.kind = "dll"
package.language = "c++"

-- Flags and Settings

	package.includepaths =
	{
		"../include",
		"."
	}
	
	package.buildflags =
	{
		"no-64bit-checks",
		"no-rtti"
	}
	
	package.defines =
	{
		"GUT_DLL",
	}
	
	package.config["Debug"].defines =
	{
		"_DEBUG"
	}

	package.config["Release"].defines =
	{
		"NDEBUG"
	}
	
	if (OS == "windows") then
		tinsert(package.defines, {"_WIN32", "WIN32"})
	end
	
	if (OS == "linux") then	
		package.linkoptions = { "-Wl,-soname -Wl,'$${ORIGIN}/libGameGuts.so'" }
	end
	
	
-- Libraries

		if (OS == "windows") then
		  tinsert(package.links, { "user32", "gdi32", "opengl32", "glu32", "dinput8", "dxguid", "winmm" })
		end
		
		if (OS == "linux") then
			tinsert(package.libpaths, findlib("X11"))
			tinsert(package.links, { "X11", "GL", "GLU" })
		end


-- Files

	package.files = 
	{
		matchfiles("../include/gut/*.h"),
		matchfiles("core/*.h", "core/*.cpp"),
	}

	if (not options["no-platform"]) then
		tinsert(package.files, matchfiles("platform/*.h", "platform/*.cpp"))
		if (OS == "windows") then
			tinsert(package.files, matchfiles("platform/msw/*.h", "platform/msw/*.cpp"))
		end
		if (OS == "linux") then
			tinsert(package.files, matchfiles("platform/x11/*.h", "platform/x11/*.cpp"))
		end
	end
	