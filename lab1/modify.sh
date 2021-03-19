#!/bin/bash

option="non empty"
recursion="False"
while getopts :lur flag #problem -> it is possible to make -lu flag! which is an error
do
    case "${flag}" in #remember to make it impossible to choose -lu
	l) option="[:lower:]" ;; #performing lowering the letter
	u) option="[:upper:]" ;; #performing uppering the letter
	r) recursion="True" ;; #performing l/u recursively
    esac
done

echo $option
echo $recursion

