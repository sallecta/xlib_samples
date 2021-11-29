#!/usr/bin/env bash


me=$(basename "$0")
dir0="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
dirSrc="$dir0/src"
dirBuild="$dir0/out"
dirBuildExe="$dirBuild/exe"
dirBuildObj="$dirBuild/obj"

source $dir0/tools/shell_fn.sh

#fn_dir_del_if $dirBuild
    fn_stoponerror "$?" $LINENO 0show 0clear

mkdir -p $dirBuildObj
    fn_stoponerror "$?" $LINENO 0show 0clear

mkdir -p $dirBuildExe
    fn_stoponerror "$?" $LINENO 0show 0clear

cp -r "$dirSrc/resources" $dirBuildExe
    fn_stoponerror "$?" $LINENO 0show 0clear

srcName="cursor"
gcc $dirSrc/$srcName.c -c -o $dirBuildObj/$srcName.o -lX11
    fn_stoponerror "$?" $LINENO 0show 0clear
gcc $dirBuildObj/$srcName.o -o $dirBuildExe/$srcName -lX11
    fn_stoponerror "$?" $LINENO 0show 0clear

srcName="draw_pixels"
gcc $dirSrc/$srcName.c -c -o $dirBuildObj/$srcName.o -lX11
    fn_stoponerror "$?" $LINENO 0show 0clear
gcc $dirBuildObj/$srcName.o -o $dirBuildExe/$srcName -lX11
    fn_stoponerror "$?" $LINENO 0show 0clear

srcName="draw_pixmap"
gcc $dirSrc/$srcName.c -c -o $dirBuildObj/$srcName.o -lX11
    fn_stoponerror "$?" $LINENO 0show 0clear
gcc $dirBuildObj/$srcName.o -o $dirBuildExe/$srcName -lX11
    fn_stoponerror "$?" $LINENO 0show 0cleardraw_pixmap

srcName="draw_text"
gcc $dirSrc/$srcName.c -c -o $dirBuildObj/$srcName.o -lX11
    fn_stoponerror "$?" $LINENO 0show 0clear
gcc $dirBuildObj/$srcName.o -o $dirBuildExe/$srcName -lX11
    fn_stoponerror "$?" $LINENO 0show 0cleardraw_pixmap

srcName="events"
gcc $dirSrc/$srcName.c -c -o $dirBuildObj/$srcName.o -lX11
    fn_stoponerror "$?" $LINENO 0show 0clear
gcc $dirBuildObj/$srcName.o -o $dirBuildExe/$srcName -lX11
    fn_stoponerror "$?" $LINENO 0show 0cleardraw_pixmap

srcName="simple_window"
gcc $dirSrc/$srcName.c -c -o $dirBuildObj/$srcName.o -lX11
    fn_stoponerror "$?" $LINENO 0show 0clear
gcc $dirBuildObj/$srcName.o -o $dirBuildExe/$srcName -lX11
    fn_stoponerror "$?" $LINENO 0show 0cleardraw_pixmap

srcName="simple_wm_hints"
gcc $dirSrc/$srcName.c -c -o $dirBuildObj/$srcName.o -lX11
    fn_stoponerror "$?" $LINENO 0show 0clear
gcc $dirBuildObj/$srcName.o -o $dirBuildExe/$srcName -lX11
    fn_stoponerror "$?" $LINENO 0show 0cleardraw_pixmap

srcName="stbimage_nanosvg"
gcc $dirSrc/$srcName.c -c -o $dirBuildObj/$srcName.o -lX11
    fn_stoponerror "$?" $LINENO 0show 0clear
gcc $dirBuildObj/$srcName.o -o $dirBuildExe/$srcName -L/usr/lib -L/usr/X11/lib -lX11 -lm
    fn_stoponerror "$?" $LINENO 0show 0cleardraw_pixmap

srcName="window_operations"
gcc $dirSrc/$srcName.c -c -o $dirBuildObj/$srcName.o -lX11
    fn_stoponerror "$?" $LINENO 0show 0clear
gcc $dirBuildObj/$srcName.o -o $dirBuildExe/$srcName -L/usr/lib -L/usr/X11/lib -lX11 -lm
    fn_stoponerror "$?" $LINENO 0show 0cleardraw_pixmap

srcName="window_transparent"
gcc $dirSrc/$srcName.c -c -o $dirBuildObj/$srcName.o -lX11
    fn_stoponerror "$?" $LINENO 0show 0clear
gcc $dirBuildObj/$srcName.o -o $dirBuildExe/$srcName -L/usr/lib -L/usr/X11/lib -lX11 -lm
    fn_stoponerror "$?" $LINENO 0show 0cleardraw_pixmap
