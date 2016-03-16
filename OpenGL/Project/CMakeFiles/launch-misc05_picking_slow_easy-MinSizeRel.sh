#!/bin/sh
bindir=$(pwd)
cd /Users/jango/Documents/GIT/AR_VR/ComputerGraphic/OpenGL/Src/misc05_picking/
export 

if test "x$1" = "x--debugger"; then
	shift
	if test "x" = "xYES"; then
		echo "r  " > $bindir/gdbscript
		echo "bt" >> $bindir/gdbscript
		GDB_COMMAND-NOTFOUND -batch -command=$bindir/gdbscript  /Users/jango/Documents/GIT/AR_VR/ComputerGraphic/OpenGL/Project/MinSizeRel/misc05_picking_slow_easy 
	else
		"/Users/jango/Documents/GIT/AR_VR/ComputerGraphic/OpenGL/Project/MinSizeRel/misc05_picking_slow_easy"  
	fi
else
	"/Users/jango/Documents/GIT/AR_VR/ComputerGraphic/OpenGL/Project/MinSizeRel/misc05_picking_slow_easy"  
fi
