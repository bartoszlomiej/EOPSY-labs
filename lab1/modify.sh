#!/bin/bash

option="non empty"
recursion="False"


#important: use the ${file##*.} to get the file extension!!!


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

while getopts ":lur:h" flag #problem -> it is possible to make -lu flag! which is an error
do
    case "${flag}" in #remember to make it impossible to choose -lu
	l) option="[:upper:] [:lower:]";; #performing lowering the letter
	u) option="[:lower:] [:upper:]";; #performing uppering the letter
	r) recursion="True" ;; #performing l/u recursively
	h) Help
    esac
done

#find -depth -type f //this command is used for finding files recursively

#find -depth -type f -exec tr $option {} \; #to change files names there is a need to use mv!

#find $recursion -type f | tr $option

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
		filename=`echo $filename | sed $2`
	fi
	newname=$filename$dot$fileextension #name of a file and it's unchanged extension
	variable=`basename $1`
	if [[ `basename $1` != $newname ]]; then
		mv $path$variable $path$newname #renaming the old file name to the new (edited) one.
	fi
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
if [ ! -d "$2" ] && [ ! -f "$2" ] && [[ ! "$2" == "" ]]; then #here it breaks
	sd="$2"
fi
while test "$1" != "" #iteration over parameters -> so as to perform ./modify on specified files/directories
do
	if [ -d "$1" ]; then
		traverse `realpath $1` $sd
	fi
	if [ -f "$1" ]; then
		change_name $1 $sd
	fi
	shift
done
#todo:
#1) Problem with flags -> if more then one is used -l -r then the sed is not working - because flags are treated as the sed input and it must be ommited
#2) fix getopts
#3) extensive modify_examples
