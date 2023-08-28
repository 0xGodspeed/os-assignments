#!/bin/sh

if [ ! -e "input.txt" ]; then
	echo "input file not found"
	exit
fi

mkdir -p Result
touch Result/output.txt
> Result/output.txt
while read -r x y operation; do
	if [[ ! $x || ! $y || ! $operation ]]; then
		echo "invalid input"
		exit
	fi

	case "$operation" in 
		"xor")
			result=$((x ^ y))
			;;
		"product")
			result=$((x * y))
			;;
		"compare")
			result=$(( x > y ? x : y ))
			;;
	esac
	echo "$x $y $operation = $result" >> Result/output.txt

done < "input.txt"
