#!/bin/bash

formats=( "png" "jpeg" )
test_file=$1

for format in ${formats[@]}; do
	out_file=$test_file.$format
	echo 'filename="'$out_file'";'>inc_file.qd;
	../../quickdeck inc_file.qd $test_file
	identify -quiet -format "%#" $out_file > $out_file.exp
	rm inc_file.qd $out_file.d
done

feh $test_file.*
