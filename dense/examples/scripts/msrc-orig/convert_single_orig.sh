#! /usr/bin/env bash
#run with: cat /home/amir/src/PMC/DATA/MSRC_ObjCategImageDatabase_v2/Images/Test.txt| parallel "./convert_single_orig.sh {.}"

filename=$1
echo $filename
y='RESULTS/'$filename'/orig/'$filename'.ppm';
if [ -f $y ]; then
    echo $y;
    b=${y:0:${#y}-4};
    imconvert $y $b'.png'
fi
