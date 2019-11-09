#!/usr/bin/env bash

#functions
fn_echobold() {
	echo -e "\e[1m$1\e[0m"
}



fn_stoponerror () {
if [ $1 -ne 0 ]; then
        lNo=$(expr $2 - 1)
        echo "  [Error at line No $lNo. $2]"
        exit
else
   echo "    [Success]"
fi
}

fn_dirEnsure () {
if [ ! -d "$1" ]; then
  mkdir -p "$1"
fi
}

fn_dirEnsureClear () {
	if [ -d "$1" ]; then
	  rm -rf "$1"
	fi
	mkdir -p "$1"
}

fn_stoponfilenotexist () {
if [ ! -f "$1" ]; then
        echo "  [Error. File \"$1\" not exist]"
        exit
fi
}

fn_run() {
	app=$1
    set -x #echo on
	$app
	excode=$?
	set +x #echo off
	return $excode
}

fn_rundetached() {
	app=$1
    set -x #echo on
	nohup $app > /dev/null &
	excode=$?
	set +x #echo off
	return $excode
}



fn_usagehelp () {
printf "\n"
printf "Build instructions


Build all:

    ./build.sh make


Clean build directory

    ./build.sh clean

"
}




#end functions
