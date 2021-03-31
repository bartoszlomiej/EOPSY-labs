#!/bin/bash

rm -rf "##test##"

sth_happend="false"

create_dir () {
	mkdir "$1"
	cd "$1"
	touch maleliterki
	touch DUZELITERKI
	touch Misiek.txt
	touch archive.tar.gz2
}

initial () {
	create_dir "##test##"
	create_dir "##test2##"
	cd ..
	create_dir "##test3##"
	create_dir "##test4##"
	echo "current directory is .../##test##/##test3##/##test4##"
}

initial

test_single_dir (){
	echo "."
	echo "."
	echo "."
	echo "showing simple case of lowerazing: modify.sh -l ."
	ls
	echo "after modification:"
	modify.sh -l . #showing the typical case
	ls
	sth_happend="true"
}

test_files () {
	echo "."
	echo "."
	echo "."
	echo "showing specific files modification: modify.sh -l DUZELITERKI ../Misiek.txt"
	echo "before:"
	ls
	echo "after:"
	modify.sh -l DUZELITERKI ../Misiek.txt
	echo "in the ##test4##"
	ls
	cd ..
	echo "in the ##test3##"
	ls
	cd "##test4##"
	sth_happend="true"
}

test_recursion (){
	cd ../..
	echo "."
	echo "."
	echo "."
	echo "current directory is .../##test##/"
	echo "showing simple case of uppering with recursion: modify.sh -ur ."
	ls
	modify.sh -ur . #showing the recursion
	echo "--after modification:"
	ls
	cd "##test2##"
	echo "--edited ##test2##:"
	pwd
	ls
	cd "../##test3##"
	echo "--edited ##test3##:"
	pwd
	ls
	cd "##test4##"
	echo "--edited ##test4##, which is inside ##test3##:"
	pwd 
	ls
	sth_happend="true"
}

test_sed () {
	echo "."
	echo "."
	echo "."
	echo "current directory is ##test4##"
	pwd
	echo "showing simple case of using sed with recursion: modify.sh -r 's/[A-Z]*[a-z]*/(&)/' .."
	echo "here the recursion will be applied to ##test3##."
	ls
	echo "--the new file news should be in parenthesis"
	modify.sh -r 's/[A-Z]*[a-z]*/(&)/' .. #showing the sed
	echo "--after modification"
	ls
	echo "--Changing directory to ##test3##"
	cd ..
	pwd
	echo "In ##test3## files are:"
	ls
	echo "However in ##test2## and ##test## files remain unchanged"
	cd "../##test2##"
	ls
	cd ".."
	ls
	cd "##test3##/##test4##" #the default route
	sth_happend="true"
}

Help () {
	echo "Shows simple examples of usage of the modify command"
	echo ""
	echo "flags:"
	echo "-d --shows the example of usage in single directory"
	echo "-r --shows the example of the recursive usage"
	echo "-s --shows the example of usage of sed pattern"
	echo "-f --shows the example of usage on specific files"
	sth_happend="true"
}

while getopts ":drshf" flag
do
	case "${flag}" in
		d) test_single_dir;;
		r) test_recursion;;
		s) test_sed;;
		f) test_files;;
		h) Help;;
	esac
done

if [[ $sth_happend == "false" ]]; then
	Help
fi
