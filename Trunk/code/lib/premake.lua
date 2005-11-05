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
	
	package.config["Release"].buildflags =
	{
		"no-frame-pointer"
	}

	
-- Defined Symbols
	
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
	
	if (windows) then
		table.insert(package.defines, {"_WIN32", "WIN32"})
	end
	
	
-- Crazy linker options to allow DLL to live in same directory as executable
	
	if (linux) then	
		package.linkoptions = { "-Wl,-soname -Wl,'$${ORIGIN}/libGameGuts.so'" }
	end
	
	
-- Libraries

		if (windows) then
		  print("In windows block")
		  table.insert(package.links, { "user32", "gdi32", "opengl32", "glu32", "dinput8", "dxguid", "winmm" })
		end
		
		if (linux) then
		   print("In linux block")
			table.insert(package.libpaths, findlib("X11"))
			table.insert(package.links, { "X11", "GL", "GLU" })
		end


-- Files

	package.files = 
	{
		matchfiles("../include/gut/*.h"),
		matchfiles("core/*.h", "core/*.cpp"),
	}

	if (not options["no-platform"]) then
		table.insert(package.files, matchfiles("platform/*.h", "platform/*.cpp"))
		if (windows) then
			table.insert(package.files, matchfiles("platform/msw/*.h", "platform/msw/*.cpp"))
		end
		if (linux) then
			table.insert(package.files, matchfiles("platform/x11/*.h", "platform/x11/*.cpp"))
		end
	end
	