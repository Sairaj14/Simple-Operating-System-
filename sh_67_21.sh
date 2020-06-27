#!/bin/bash
read -p "ENter name:" name
read -p	"Enter marks of subject 1:" sub1
read -p	"Enter marks of subject 2:" sub2
read -p	"Enter marks of subject 3:" sub3

total=$((sub1+sub2+sub3))
percentage=$((total/3))
echo "$name's total marks:-$total"
echo "$name's percentage:-$percentage%"

if [$percentage -gt 75 -a $percentage -lt 100]
	then
	echo "you got First class"
if [$percentage -lt 75 -a $percentage -gt 40]
	then
	echo "you got Second class"
else
	echo "you failed"
fi
