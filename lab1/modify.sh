#!/bin/bash

option="non empty"
recursion="False"


#important: use the ${file##*.} to get the file extension!!!


Help() {
	echo "This program modifies the file names to upper/lower letters."
	echo "Usage of flags:"
	echo "-l	lowering the file names in current directory"
	echo "-u	uppering the file names in current directory"
	echo "add additonaly flag -r to do it recursively in all directories"
	echo "example:"
	echo "modify -ur		uppering all file names in recursively from current directory"
	
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

change_name() {
	filename=`basename $1 | cut -f1 -d '.'` #| tr $option`
	fileextension=`basename ${1##*.}`  #| cut -f2 -d '.'`
       	echo "extension: $fileextension"
	echo "filename: $filename"
	if [[ $filename == $fileextension ]]; then
		fileextension=""
		dot=""
	else
		dot="."
	fi
	filename=`basename $1 | cut -f1 -d '.' | tr $option`

	newname=$filename$dot$fileextension
	if [[ `basename $1` != $newname ]]; then
		mv $2/$1 $2/$newname #here is the problem for now
	fi
}

traverse () {
	local a file #a is the first argument of a function. The a is used instead of $1 as it is impossible to iterate over $1
	for a; do
		for file in "$a"/*; do
			if [[ -d $file ]]; then
				if [[ $recursion == True ]]; then
					traverse "$file"
				fi
			else
				onlyname=`readlink -f $file`
				change_name $onlyname $a
			fi
		done
	done
}
#traverse `pwd`

while test "$1" != ""
do
	echo "parameters: $1"
	if [ -d "$1" ]; then
		echo "dziala $1"
	fi
	if [ -f "$1" ]; then
		pwd
		change_name $1 `pwd`
	fi	
	shift
done
#todo:
#0) give the directory/file names directly to lower/upper
#1) sed
#2) fix getopts
#3) 

#How to do the #0?
#solution: -read all commandline parameters. Check if it is a file or directory. if file => apply the function lower/upper. If directory => function traverse 'directory path' (directory path = `pwd`/directoryname) 

#current issue: when the file is test/Misiek.txt then mv tries to mv `pwd`/Misiek.tx instead of `pwd`/test/Misiek.txt to `pwd`/MISIEK.txt instead of `pwd`/test/MISIEK.txt

