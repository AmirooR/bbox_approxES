#! /bin/bash
filename=$1
export NUM=`grep -iHn e1 $filename |cut -f2 -d':'`
awk -v num=$NUM 'NR > num' $filename
