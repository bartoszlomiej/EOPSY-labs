#!/bin/bash

option="non empty"
recursion="-maxdepth 0"
while getopts :lur flag #problem -> it is possible to make -lu flag! which is an error
do
    case "${flag}" in #remember to make it impossible to choose -lu
	l) option="[:upper:] [:lower:]";; #performing lowering the letter
	u) option="[:lower:] [:upper:]";; #performing uppering the letter
	r) recursion="-depth" ;; #performing l/u recursively
    esac
done

#find -depth -type f //this command is used for finding files recursively

#find -depth -type f -exec tr $option {} \; #to change files names there is a need to use mv!

#find $recursion -type f | tr $option

traverse_directories () {
	for f in *; do
		if [ -d "$f" ]; then
			echo $f #tutaj jest directory
		else
			filename=`echo $f | cut -f1 -d '.'` #| tr $option`
			fileextension=`echo ${f##*.}`  #| cut -f2 -d '.'` 
			if [[ $filename == $fileextension ]]; then
				fileextension=""
				dot=""
			else
				dot="."
			fi
			filename=`echo $f | cut -f1 -d '.' | tr $option`

			newname=$filename$dot$fileextension
			mv $f $newname
		fi
	done
}

#traverse_directories

traverse () {
	local a file #a is the first argument of a function. The a is used instead of $1 as it is impossible to iterate over $1
	for a; do
		for file in "$a"/*; do
			if [[ -d $file ]]; then
				traverse "$file"
			else
		#		basename $file
				filename=`basename $file | cut -f1 -d '.'` #| tr $option`
				fileextension=`basename ${file##*.}`  #| cut -f2 -d '.'` 
				if [[ $filename == $fileextension ]]; then
					fileextension=""
					dot=""
				else
					dot="."
				fi
				filename=`basename $file | cut -f1 -d '.' | tr $option`
	
				newname=$filename$dot$fileextension
				mv `basename $file` $newname #here is an error - the mv command runs in parent directory
			fi
		done
	done
}
traverse `pwd` 

#important TODOS: 1) get to know how to list all files in that way, that it is possible to use tr command, and further use the mv command (mv oldname NEWNAME), where NEWNAME is the output of the tr, and oldname is input to tr
