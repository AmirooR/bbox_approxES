#! /usr/bin/env bash

#run with: cat Train.txt| parallel "./compare_single.sh {}"

newgt_dir=~/Downloads/boundary_GT/ #/Users/amirrahimi/Downloads/newgt/
lasso_dir=~/Downloads/boundary_GT_lasso/
filename_ext=$1
filename="${filename_ext%.*}"
echo $filename
for y in `ls $PWD'/RESULTS/'$filename'/l_-0.5_0.5/'*_output.ppm`; do
    if [ -f $y ]; then
        echo $y;
        b=${y:0:${#y}-4};
        x=$newgt_dir$filename'.bmp'
        w=$lasso_dir$filename'.bmp'
        if [ -f $y ]; then
            /media/amir/Data/BACKUP/temp/compare/compare_lasso $x $w $y > $b'.txt';
        fi
        #/home/amir/temp/compare/compare $x $y > $b'.txt';
    fi    
done
                                            
