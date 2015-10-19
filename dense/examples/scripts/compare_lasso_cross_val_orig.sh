#! /usr/bin/env bash

newgt_dir=~/Downloads/boundary_GT/ #/Users/amirrahimi/Downloads/newgt/
lasso_dir=~/Downloads/boundary_GT_lasso/
cross_val_dir=CROSSVAL-UNCOMPRESSED-COORD/
for x in `ls $newgt_dir*.bmp`; do
    a=${x:0:${#x}-4};
    filename=$(basename $a); 
    echo $filename
    for z in `ls $cross_val_dir`; do
        if [ -d $cross_val_dir$z ];then
            y=$cross_val_dir$z'/'$filename'/'$filename'.ppm';
            w=$lasso_dir$filename'.bmp';
            if [ -f $y ]; then
                b=${y:0:${#y}-4};
                /media/amir/Data/BACKUP/temp/compare/compare_lasso $x $w $y > $b'.txt';
            fi
            echo $cross_val_dir$z
        fi
    done
done
