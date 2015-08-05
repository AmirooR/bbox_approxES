#! /usr/bin/env bash
#run with: cat /home/amir/src/PMC/DATA/MSRC_ObjCategImageDatabase_v2/Images/Test.txt| parallel "./compare_single.sh {.}"
gt_dir=/home/amir/src/PMC/DATA/MSRC_ObjCategImageDatabase_v2/Images/
filename=$1
echo $filename
for y in `ls $PWD'/RESULTS/'$filename'/l_0.0_20/'*_output.ppm`; do
    if [ -f $y ]; then
        echo $y;
        b=${y:0:${#y}-4};
        x=$gt_dir$filename'_GT.bmp'
        /home/amir/temp/compare/compare $x $y > $b'.txt';
    fi
done
