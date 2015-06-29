#! /usr/bin/env bash

#img_dir=/home/amir/src/PMC/DATA/MSRC_ObjCategImageDatabase_v2/Images/ppms/ #/Users/amirrahimi/Downloads/MSRC_ObjCategImageDatabase_v2/Images/ppms/
#unary_dir=/home/amir/src/PMC/DATA/msrc_compressed/ #/Users/amirrahimi/Downloads/msrc_compressed/
newgt_dir=/home/amir/src/PMC/DATA/newgt/ #/Users/amirrahimi/Downloads/newgt/

for x in `ls $newgt_dir*.bmp`; do
    a=${x:0:${#x}-7};
    filename=$(basename $a); 
    #unary=$unary_dir$filename'.c_unary';
    #`mkdir -p RESULTS/$filename/l_0.0_20/`
    echo $filename
    in_dir='RESULTS/'$filename'/l_0.0_20/';
    ./get_best.py $in_dir
done
    #./dense_inference_pr_probimage $img_dir$filename'.ppm' $unary_dir$filename'.c_unary' 'RESULTS/'$filename'/l_0.0_20/'>'RESULTS/'$filename'/l_0.0_20/output.txt'
    #echo $img_dir$filename'.ppm' $unary_dir$filename'.c_unary' 'RESULTS/'$filename'/l_0.0_20/'>'RESULTS/'$filename'/l_0.0_20/output.txt'
