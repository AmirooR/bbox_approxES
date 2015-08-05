#! /usr/bin/env bash

#run with: cat /home/amir/src/PMC/DATA/MSRC_ObjCategImageDatabase_v2/Images/Test.txt| parallel "./convert_single.sh {.}"

test_list=/home/amir/src/PMC/DATA/MSRC_ObjCategImageDatabase_v2/Images/Test.txt
filename=$1
echo $filename
for y in `ls 'RESULTS/'$filename'/l_0.0_20/'*_output.ppm`;do
    if [ -f $y ]; then
        echo $y;
        b=${y:0:${#y}-4};
        imconvert $y $b'.png'
    fi
done
