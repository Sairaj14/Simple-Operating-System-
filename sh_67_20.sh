#!/bin/bash
read -p "ENter name :" str
len=$(echo -n $str | wc -m)
echo "hi $str"
echo "ur name is of $len characters"
echo $str|rev
