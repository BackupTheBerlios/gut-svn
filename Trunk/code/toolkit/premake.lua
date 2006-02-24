package.name = "GameGuts"
package.kind = "dll"
package.language = "c++"

-- Figure out my target platform

	local platform
	if (windows) then
		platform = "msw"
	end
	if (linux or options["with-x11"]) then
		platform = "x11"
	end
	

-- Flags and Settings

	package.includepaths =
	{
		"../../include",
		"."
	}
	
	package.buildflags =
	{
		"no-64bit-checks",
		"no-rtti"
	}
	
	package.config["Release"].buildflags =
	{
		"no-frame-pointer",
		"no-symbols",
		"optimize-speed",
	}

	-- For Cygwin DirectX
	if (platform == "msw" and options["target"] == "gnu") then
		tinsert(package.includepaths, "/usr/include/directx")
	end
	


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
		tinsert(package.defines, {"_WIN32", "WIN32"})
	end
	
	if (options["no-platform"]) then
		tinsert(package.defines, "NO_PLATFORM")
	end
	
	if (options["no-graphics"]) then
		tinsert(package.defines, "NO_GRAPHICS")
	end
	
	if (options["with-x11"]) then
		tinsert(package.defines, "FORCE_X11")
		if (windows) then  -- hack to get things working right now
			tinsert(package.includepaths, "/usr/X11R6/include")
			tinsert(package.libpaths, "/usr/X11R6/lib")
		end
	end
	
	
-- Crazy linker options to allow DLL to live in same directory as executable
	
	if (linux) then	
		package.linkoptions = { "-Wl,-soname -Wl,'$${ORIGIN}/libGameGuts.so'" }
	end
	
	
-- Libraries

	if (not options["no-platform"]) then
		if (platform == "msw") then
			tinsert(package.links, { "user32", "gdi32", "dinput", "dxguid", "winmm" })
		end
		if (platform == "x11") then
			tinsert(package.libpaths, findlib("X11"))
			tinsert(package.links, { "X11" })
		end
	end

	if (not options["no-graphics"]) then
		if (platform == "msw") then
			tinsert(package.links, { "opengl32", "glu32" })
		end
		if (platform == "x11") then
			tinsert(package.links, { "GL", "GLU" })
		end
	end
	
	
-- Files

	function addmodule(module)
		tinsert(package.files, matchfiles(module.."/*.h", module.."/*.cpp"))
		tinsert(package.files, matchfiles(module.."/"..platform.."/*.h", module.."/"..platform.."/*.cpp"))
	end
	
	package.files = 
	{
		matchfiles("../../include/gut/*.h"),
	}

	addmodule("core")
	addmodule("math")

	if (not options["no-platform"]) then
		addmodule("platform")
	end
	
	if (not options["no-graphics"]) then
		addmodule("graphics")
		addmodule("graphics/gl")
	end
	
