#! /usr/bin/env bash

newgt_dir=/home/amir/src/PMC/DATA/MSRC_ObjCategImageDatabase_v2/Images/

for x in `ls $newgt_dir*_GT.bmp`; do
    a=${x:0:${#x}-7};
    filename=$(basename $a); 
    y='RESULTS/'$filename'/orig/'$filename'.ppm';
    if [ -f $y ]; then
        b=${y:0:${#y}-4}'.txt';
        cat $b;
    fi
done
