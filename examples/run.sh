#!/bin/bash
cd ./in
for file in ./*.tmvm
do
	name=${file%.*}
	name=${name#*/}
	../../Debug/VirtualMachine $file ./ 1 > ../out/$name.out
	diff ../out/$name.out ../out/$name.txt
	echo $name
	rm ../out/$name.out
done;