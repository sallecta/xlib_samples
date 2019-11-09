#!/usr/bin/env bash

#configs
dirbuild="$PWD/out"
dirbuildexe="$dirbuild/exe"
dirbuildobj="$dirbuild/obj"
srcdir="$PWD/src"
srcMainMarker="00.c" # if file ends with ".$srcMainMarker", it will be processed
srcFilesMask="$srcdir/*.$srcMainMarker"
srcCustomLinkerObjMark="$srcMainMarker.customLinkerObj.sh"
srcCustomLinkerExeMark="$srcMainMarker.customLinkerExe.sh"
optionsLinkerObjCommon="-lX11"
optionsLinkerExeCommon="-lX11"

#end configs

source "$PWD/include_scripts/functions.sh"

fn_buildobj() {
	me="fn_buildobj"
	name=$1
	printf "\n"
	fn_echobold "    $me: Creating obj file from $name.$srcMainMarker"
    customLinkerFile="$srcdir/$name.$srcCustomLinkerObjMark"
	if [ ! -f "$customxLinkerFile" ]; then
        echo "    $me: No custom linker options file found [$customLinkerFile]"
		optionsLinker=$optionsLinkerObjCommon
	else
		echo "    $me:  Custom llinker options file found [$customLinkerFile]"
		source $customLinkerFile
		fn_stopifempty "$optionsLinkerCustom" "optionsLinkerCustom"
		optionsLinker="$optionsLinkerCustom" # located in $customLinkerFile
	fi
	cmd="gcc $srcdir/$name.$srcMainMarker -c -o $dirbuildobj/$name.o $optionsLinker"
	echo $cmd
	$cmd
	excode=$?

	optionsLinker=""
	optionsLinkerCustom=""

	return $excode
}

fn_buildexe() {
	me="fn_buildexe"
	name=$1
	printf "\n"
	fn_echobold "    $me: Creating exe file from $name..$srcMainMarker"

    customLinkerFile="$srcdir/$name.$srcCustomLinkerExeMark"
	if [ ! -f "$customLinkerFile" ]; then
        echo "    $me: No custom linker options file found [$customLinkerFile]"
		optionsLinker=$optionsLinkerExeCommon
	else
		echo "    $me:  Custom llinker options file found [$customLinkerFile]"
		source $customLinkerFile
		fn_stopifempty "$optionsLinkerCustom" "optionsLinkerCustom"
		optionsLinker="$optionsLinkerCustom" # located in $customLinkerFile
	fi

	cmd="gcc $dirbuildobj/$name.o -o $dirbuildexe/$name $optionsLinker"
	echo $cmd
	$cmd
	excode=$?

	optionsLinker=""
	optionsLinkerCustom=""

	return $excode
}

fn_makeInDir () {
	me="fn_makeInDir"
    fn_dirEnsureClear "$dirbuildexe"
    fn_dirEnsureClear "$dirbuildobj"
	shopt -s nullglob
	array=($srcFilesMask)
	if [ ${#array[@]} -eq 0 ]; then
		echo "    $me: Error: No target files with mask $srcFilesMask. Exit."
		exit 1
	fi
	for file in "${array[@]}"
	do
		target=$(basename ${file})
		target=${target%.$srcMainMarker}
		target=${target##*/}
		me="fn_makeInDir"
		fn_echobold "\n\n$me: found target: $target\n"
		fn_buildobj "$target"
		fn_stoponerror "$?" $LINENO
		me="fn_makeInDir"
		fn_buildexe "$target"
		fn_stoponerror "$?" $LINENO		  
	done
	fn_cpresources "$srcdir/resources"
	fn_stoponerror "$?" $LINENO	
	
}

fn_cpresources() {
	me="fn_cpresources"
	name=$1
	printf "\n"
	fn_echobold "$me: Copying resource folder $name to exe dir"
	cmd="cp -r $name $dirbuildexe"
	echo $cmd
	$cmd
	excode=$?
	return $excode
}

me="main script"

if [ "$1" = "--make" ] || [ "$1" = "make" ]; then
	if [ ! "$2" = "" ]; then
		target="$2"
		fn_stoponfilenotexist "$srcdir/$target.$srcMainMarker"
		fn_dirEnsure "$dirbuildobj"
		fn_echobold "\n\n$me: found target: $target\n"
		fn_buildobj "$target"
		fn_stoponerror "$?" $LINENO	
		fn_dirEnsure "$dirbuildexe"
		fn_buildexe "$target"
		fn_stoponerror "$?" $LINENO	
		exit
	else
		fn_dirEnsureClear "$dirbuildexe"
		fn_dirEnsureClear "$dirbuildobj"
		fn_makeInDir
	fi


elif [ "$1" = "--clean" ] || [ "$1" = "clean" ]; then 
    rm -rf $dirbuild
    mkdir -p $dirbuild

elif [ "$1" = "--help" ] || [ "$1" = "help" ]; then 
    fn_usagehelp	

else
    fn_echobold "Wrong argument."
	fn_usagehelp
fi
