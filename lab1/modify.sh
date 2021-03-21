#!/bin/bash

option="non empty"
recursion="False"
while getopts :lur flag #problem -> it is possible to make -lu flag! which is an error
do
    case "${flag}" in #remember to make it impossible to choose -lu
	l) option="[:upper:] [:lower:]";; #performing lowering the letter
	u) option="[:lower:] [:upper:]";; #performing uppering the letter
	r) recursion="True" ;; #performing l/u recursively
    esac
done

#find -depth -type f //this command is used for finding files recursively

#find -depth -type f -exec tr $option {} \; #to change files names there is a need to use mv!

#find $recursion -type f | tr $option

traverse () {
	local a file #a is the first argument of a function. The a is used instead of $1 as it is impossible to iterate over $1
	for a; do
		for file in "$a"/*; do
			if [[ -d $file ]]; then
				if [[ $recursion == True ]]; then
					traverse "$file"
				fi
			else
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
				mv $a/`basename $file` $a/$newname #here is an error - the mv command runs in parent directory
			fi
		done
	done
}
traverse `pwd` 
