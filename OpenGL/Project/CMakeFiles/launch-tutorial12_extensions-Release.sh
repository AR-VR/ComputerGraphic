#!/bin/sh
bindir=$(pwd)
cd /Users/jango/Documents/GIT/AR_VR/ComputerGraphic/OpenGL/Src/tutorial12_extensions/
export 

if test "x$1" = "x--debugger"; then
	shift
	if test "x" = "xYES"; then
		echo "r  " > $bindir/gdbscript
		echo "bt" >> $bindir/gdbscript
		GDB_COMMAND-NOTFOUND -batch -command=$bindir/gdbscript  /Users/jango/Documents/GIT/AR_VR/ComputerGraphic/OpenGL/Project/Release/tutorial12_extensions 
	else
		"/Users/jango/Documents/GIT/AR_VR/ComputerGraphic/OpenGL/Project/Release/tutorial12_extensions"  
	fi
else
	"/Users/jango/Documents/GIT/AR_VR/ComputerGraphic/OpenGL/Project/Release/tutorial12_extensions"  
fi
