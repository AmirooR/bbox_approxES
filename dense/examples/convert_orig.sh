#! /usr/bin/env bash

newgt_dir=/home/amir/src/PMC/DATA/newgt/ #/Users/amirrahimi/Downloads/newgt/

for x in `ls $newgt_dir*.bmp`; do
    a=${x:0:${#x}-7};
    filename=$(basename $a); 
    echo $filename
    y='RESULTS/'$filename'/orig/'$filename'.ppm';
    if [ -f $y ]; then
        echo $y;
        b=${y:0:${#y}-4};
        imconvert $y $b'.png'
    fi
done
