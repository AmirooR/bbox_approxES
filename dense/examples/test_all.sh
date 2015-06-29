#! /usr/bin/env bash

img_dir=/Users/amirrahimi/Downloads/MSRC_ObjCategImageDatabase_v2/Images/ppms/
unary_dir=/Users/amirrahimi/Downloads/msrc_compressed/
newgt_dir=/Users/amirrahimi/Downloads/newgt/

for x in `ls $newgt_dir*.bmp`; do
    a=${x:0:${#x}-7};
    filename=$(basename $a); 
    unary=$unary_dir$filename'.c_unary';
    `mkdir -p RESULTS/$filename/l_0.0_20/`
    echo $filename
    ./dense_inference_pr_probimage $img_dir$filename'.ppm' $unary_dir$filename'.c_unary' 'RESULTS/'$filename'/l_0.0_20/'>'RESULTS/'$filename'/l_0.0_20/output.txt'
done
