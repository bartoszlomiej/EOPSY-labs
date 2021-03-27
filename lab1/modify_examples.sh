#!/bin/bash

rm -rf "##test##"
create_dir () {
	mkdir "$1"
	cd "$1"
	touch maleliterki
	touch DUZELITERKI
	touch Misiek.txt
	touch archive.tar.gz2
}

create_dir "##test##"
create_dir "##test2##"
cd ..
create_dir "##test3##"
create_dir "##test4##"
echo "current directory is .../##test##/##test3##/##test4##"
echo "showing simple case of lowerazing: modify -l ."
ls
echo "after modification:"
#modify -l . #showing the typical case
ls
cd ../..
echo "current directory is .../##test##/"
echo "showing simple case of uppering with recursion: modify -l ."
ls
#modify -ru . #showing the recursion
echo "after modification:"
ls
cd "##test2##"
pwd
cd "../##test3##"
pwd
ls
cd "##test4##"
pwd ls
cd ../..
echo "current directory is .../##test##/"
echo "showing simple case of using sed: modify 's/[A-Z]*[a-z]*/(&)/' ."
#modify -xd 's/[A-Z]*[a-z]*/(&)/'. #showing the sed
ls
