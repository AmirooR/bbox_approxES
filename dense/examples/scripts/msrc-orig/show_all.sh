#! /usr/bin/env bash

newgt_dir=/home/amir/src/PMC/DATA/MSRC_ObjCategImageDatabase_v2/Images/
#/home/amir/src/PMC/DATA/newgt/ #/Users/amirrahimi/Downloads/newgt/

for x in `ls $newgt_dir*_GT.bmp`; do
    a=${x:0:${#x}-7};
    filename=$(basename $a); 
    infile='RESULTS/'$filename'/l_0.0_20/'best_*_output.txt;
    if [ -f $infile ]; then

        cat  $infile;
    fi
done
