#!/bin/bash

success=0

for test_file in script/*.qd; do
	../quickdeck $test_file > $test_file.out
	diff -y $test_file.out $test_file.exp > $test_file.res
	res=$?
	if [ ${res} -ne 0 ]; then
		echo -e "$test_file \e[31m FAIL \e[0m"
		cat $test_file.res
		success=1;
	else
		echo -e "$test_file \e[32m PASS \e[0m"
	fi
	rm $test_file.out $test_file.res
done

if [ ${success} -eq 0 ]; then
	echo -e "\e[32mAll tests passed.\e[0m"
else
	echo -e "\e[31mTests failing.\e[0m"
fi
exit $success
