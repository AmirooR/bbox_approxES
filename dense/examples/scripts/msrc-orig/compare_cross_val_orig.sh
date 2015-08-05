#! /usr/bin/env bash

newgt_dir=/home/amir/src/PMC/DATA/newgt/ #/Users/amirrahimi/Downloads/newgt/
cross_val_dir=CROSSVAL-UNCOMPRESSED-COORD/
for x in `ls $newgt_dir*.bmp`; do
    a=${x:0:${#x}-7};
    filename=$(basename $a); 
    echo $filename
    for z in `ls $cross_val_dir`; do
        if [ -d $cross_val_dir$z ];then
            y=$cross_val_dir$z'/'$filename'/'$filename'.ppm';
            if [ -f $y ]; then
                b=${y:0:${#y}-4};
                /home/amir/temp/compare/compare $x $y > $b'.txt';
            fi
            echo $cross_val_dir$z
        fi
    done
done
