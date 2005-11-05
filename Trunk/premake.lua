-- Premake script for GameGuts project
-- See http://premake.sourceforge.net/ for more information

project.name = "GameGuts"

project.config["Debug"].bindir = "bin/debug"
project.config["Debug"].libdir = "bin/debug"

project.config["Release"].bindir = "bin/release"
project.config["Release"].libdir = "bin/release"

addoption("no-platform", "Exclude the platform abstraction API from the build")
addoption("with-tests",  "Include the test suite applications")

if (options["with-tests"]) then
	dopackage("tests/input")
	dopackage("tests/memory")
	dopackage("tests/window")
end

dopackage("code/lib")

local builtin = docommand
function docommand(cmd, ...)
	builtin(cmd, unpack(arg))
	if (cmd == "clean") then
		rmdir("bin/debug")
		rmdir("bin/release")
		rmdir("bin")
	end
end
