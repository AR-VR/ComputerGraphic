#!/bin/sh
bindir=$(pwd)
cd /Users/jango/Documents/GIT/AR_VR/ComputerGraphic/OpenGL/Src/tutorial16_shadowmaps/
export 

if test "x$1" = "x--debugger"; then
	shift
	if test "x" = "xYES"; then
		echo "r  " > $bindir/gdbscript
		echo "bt" >> $bindir/gdbscript
		GDB_COMMAND-NOTFOUND -batch -command=$bindir/gdbscript  /Users/jango/Documents/GIT/AR_VR/ComputerGraphic/OpenGL/Project/Release/tutorial16_shadowmaps 
	else
		"/Users/jango/Documents/GIT/AR_VR/ComputerGraphic/OpenGL/Project/Release/tutorial16_shadowmaps"  
	fi
else
	"/Users/jango/Documents/GIT/AR_VR/ComputerGraphic/OpenGL/Project/Release/tutorial16_shadowmaps"  
fi
