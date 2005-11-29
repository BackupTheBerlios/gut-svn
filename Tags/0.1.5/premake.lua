-- Premake script for GameGuts project
-- See http://premake.sourceforge.net/ for more information

project.name = "GameGuts"

-- Project options

	addoption("no-platform", "Exclude the platform abstraction API from the build")
	addoption("with-tests",  "Include the test suite applications")


-- Set config specific output directories

	project.config["Debug"].bindir = "bin/debug"
	project.config["Debug"].libdir = "bin/debug"

	project.config["Release"].bindir = "bin/release"
	project.config["Release"].libdir = "bin/release"


-- Packages

	if (options["with-tests"]) then
		dopackage("tests/input")
		dopackage("tests/memory")
		dopackage("tests/window")
	end

	dopackage("code/toolkit")


-- Clean up some additional files; extra code here to make it work on
-- both Premake v2.x and Premake v3.x, will phase out later

	oldcommand = docommand
	function docommand(cmd, arg)
		if (cmd == "clean") then
			doclean(cmd, arg)
		else
			oldcommand(cmd, arg)
		end
	end

	function doclean(cmd, arg)
		oldcommand(cmd, arg)
		rmdir("bin/debug")
		rmdir("bin/release")
		rmdir("bin")
	end
	