#!/bin/bash

script_dir=`pwd`

# Make sure a version is supplied
if [ $# -ne 1 ]; then
	echo 1>&2 "Usage: $0 version"
	exit 1
fi

echo "PREPARING BUILD $1"
echo ""

# Make sure all prerequisites are met
echo "Is the version ($1) in the format Major.Minor.Build?"
read line
echo ""
echo "Did you update the version, date, and project status in README.txt?"
read line
echo ""
echo "Did you update the version and date in gut.h?"
read line
echo ""
echo "Did you update CHANGES.txt?"
read line
echo ""
echo "Did you create a tag for version $1?"
read line
echo ""
echo "Have you run ALL tests on THIS platform and THIS build?"
read line
echo ""
echo "Ready to build packages for version $1."
echo "Press [Enter] to begin."
read line


#####################################################################
# Stage 1: Preparation
#
# Pull the source code from Subversion
#####################################################################

echo ""
echo "RETRIEVING SOURCE CODE FROM REPOSITORY..."
echo ""
cd ../..
svn co https://svn.berlios.de/svnroot/repos/gut/Tags/$1 GameGuts-$1


#####################################################################
# Stage 2: Source Code Package
#
# Build the source code package
#####################################################################

echo ""
echo "REMOVING PRIVATE FILES..."
echo ""

rm -rf `find . -name .svn`
rm -rf packaging
rm -rf design

echo ""
echo "PACKAGING SOURCE CODE..."
echo ""

cd ..
zip -r9 $script_dir/GameGuts-src-$1.zip GameGuts-$1/*


#####################################################################
# Stage 3: Binary Package
#####################################################################

echo ""
echo "BUILDING RELEASE BINARY..."
echo ""

cd GameGuts-$1
premake --target vs2003
"c:/Program Files/Microsoft Visual Studio .NET 2003/Common7/IDE/devenv.exe" /rebuild Release GameGuts.sln
cd bin/release
zip -9 $script_dir/GameGuts-win32-$1.zip GameGuts.dll


#####################################################################
# Stage 4: Publish Files
#
# Send the files to SourceForge
#####################################################################

cd $script_dir
echo ""
echo "Upload packages to SourceForge?"
read line
if [ $line = "y" ]; then
	echo "Uploading to SourceForge..."
	ftp -s:pkg_win_ftp.txt upload.sourceforge.net
fi


#####################################################################
# All done
#####################################################################

echo ""
echo "CLEANING UP..."
echo ""
cd ../..
rm -rf GameGuts-$1

cd $script_dir
echo ""
echo "Done."
