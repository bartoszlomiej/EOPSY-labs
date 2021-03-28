#!/bin/bash

option="non empty"
recursion="False"

Help() { #Just the function, which prints the help message
	echo "This program modifies the file names to upper/lower letters."
	echo "Usage:"
        echo "./modify <flags> <dir/file names...>"
	echo "./modify <flags> <sed pattern> <dir/file names...>"
	echo "Usage of flags:"
	echo "-l	lowering the file names in current directory"
	echo "-u	uppering the file names in current directory"
	echo "-r 	performes recursively in all directories"
	echo "-h	shows the help"
	echo "example:"
	echo "modify -ur .		uppering all file names in recursively from current directory"
	
}

while getopts "lr:ur:h" flag
do
    case "${flag}" in
	l) option="[:upper:] [:lower:]";; #performing lowering the letter
	u) option="[:lower:] [:upper:]";; #performing uppering the letter
	r) recursion="True" ;; #performing l/u recursively
	h) Help
    esac
done

change_name() { #it is a function which changes the name of a given single file depending on the flag.
	variable=`realpath $1` #buffer variable
	path=`echo "${variable%/*}/"`	#getting the path to the folder where the file is
	filename=`basename $1 | cut -f1 -d '.'` #the base name of the file
	fileextension=`basename ${1##*.}`  #getting the file extension only.
	if [[ $filename == $fileextension ]]; then #file doesn't have extension
		fileextension=""
		dot=""
	else #file have extension
		dot="."
	fi
	if [[ $2 == "true" ]]; then
		filename=`basename $1 | cut -f1 -d '.' | tr $option` #pure file name however with lower/upper letters
	else
		buffer=`echo $filename | sed $2 > /dev/null 2>&1` #getting rid of annoying sed error masseges
		if [[ "$?" -eq "0" ]]; then
			filename=`echo $filename | sed $2`
		else
			errormsg=`echo "The given sed pattern is incorrect. Exit code: $?"`
		fi
	fi
	newname=$filename$dot$fileextension #name of a file and it's unchanged extension
	variable=`basename $1`
	if [[ `basename $1` != $newname ]]; then
		mv $path$variable $path$newname #renaming the old file name to the new (edited) one.
	fi
	sth_happend="true" #for showing help if no arguments are given
}

traverse () { #function which performs recursion and iteration over all files in given directory
	local a file #a is the first argument of a function. The a is used instead of $1 as it is impossible to iterate over $1
	for a; do
		for file in "$a"/*; do
			if [[ -d $file ]]; then
				if [[ $recursion == True ]]; then
					traverse $file $2
				fi
			else
				if [ -f "$file" ]; then
					change_name $file $2
				fi
			fi
		done
	done
}

sd="true"
sth_happend="false"

while test "$1" != "" #iteration over parameters -> so as to perform ./modify on specified files/directories
do
	if [[ "$1" != "-l" ]] && [[ "$1" != "-u" ]] && [[ "$1" != "-r" ]] && [[ "$1" != "-lr" ]] && [[ "$1" != "-rl" ]] && [[ "$1" != "-ur" ]] && [[ "$1" != "-ru" ]]; then #this operation is being done to
		if [ ! -d "$1" ] && [ ! -f "$1" ] && [[ ! "$1" == "" ]]; then #skip known flags for the sed
			sd="$1"
		fi	
	fi
	if [ -d "$1" ]; then
		traverse `realpath $1` $sd
	fi
	if [ -f "$1" ]; then
		change_name $1 $sd
	fi
	shift
done

if [[ $sth_happend == "false" ]]; then #if no name was changed, the help shows
	Help
fi

if [[ $errormsg != "" ]]; then #printing the error message of the sed command (it happens often)
	echo $errormsg
fi

#todo:
#3) extensive modify_examples
