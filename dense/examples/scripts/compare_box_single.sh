#! /usr/bin/env bash

#run with: cat Train.txt| parallel "./compare_box_single.sh {}"

newgt_dir=/media/amir/Data/BACKUP/src/PMC/ParametricMaxflow/Data/boundary_GT/ #/Users/amirrahimi/Downloads/newgt/
filename_ext=$1
filename="${filename_ext%.*}"
echo $filename
for y in `ls $PWD'/RESULTS/'$filename'/l_0.0_100/'*_output.ppm`; do
    if [ -f $y ]; then
        echo $y;
        b=${y:0:${#y}-4};
        x=$newgt_dir$filename'.bmp'
        if [ -f $y ]; then
            /media/amir/Data/BACKUP/temp/compare/compare_fg_bg $x $y > $b'.txt';
        fi
        #/home/amir/temp/compare/compare $x $y > $b'.txt';
    fi    
done
                                            
