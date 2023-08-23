#!/bin/bash

mkdir -p Result
touch Result/output.txt
> Result/output.txt
while IFS=" " read -r x y operation; do
	case "$operation" in 
		"xor")
			result=$((x ^ y))
			;;
		"product")
			result=$((x * y))
			;;
		"compare")
			result=$(( a > b ? a : b ))
			;;
	esac
	echo "$x $y $operation = $result" >> Result/output.txt

done < "input.txt"
