#!/bin/sh

script_dir=`pwd`

# Make sure a version is supplied
if [ $# -ne 1 ]; then
	echo 1>&2 Usage: $0 version
	exit 1
fi
echo "PREPARING BUILD $1"
echo ""

# Make sure all prerequisites are met
echo "Is the version ($1) in the format Major.Minor.Build?"
read line
echo ""
echo "Did you already package the Windows build?"
read line
echo ""
echo "Have you run ALL tests on THIS platform and THIS build?"
read line
echo ""
echo "Ready to build package for version $1."
echo "Press [Enter] to begin."
read line


#####################################################################
# Stage 1: Preparation
#
# Pull the source code from Subversion. Version numbers should have
# already been updated by the Windows packaging script.
#####################################################################

echo ""
echo "RETRIEVING SOURCE CODE FROM REPOSITORY..."
echo ""
cd ../..
svn co https://svn.berlios.de/svnroot/repos/gut/Tags/$1 GameGuts-$1


#####################################################################
# Stage 2: Binary Package
#
# Build the binary package
#####################################################################

echo ""
echo "BUILDING RELEASE BINARY..."
echo ""

cd GameGuts-$1
premake --target gnu
make CONFIG=Release
if [ $? -ne 0 ]; then
	echo ""
	echo "** BUILD FAILED! **"
	exit 1
fi

cd bin/release
tar czvf $script_dir/GameGuts-linux-$1.tar.gz libGameGuts.so


#####################################################################
# All done
#####################################################################

echo "Cleaning up..."
cd ../../..
rm -rf GameGuts-$1
cd $script_dir

echo ""
echo "Upload package to SourceForge?"
read line
if [ $line = "y" ]; then
	echo "Uploading to SourceForge..."
	ftp -n upload.sourceforge.net < pkg_linux_ftp.txt
fi

echo ""
echo "Done."
