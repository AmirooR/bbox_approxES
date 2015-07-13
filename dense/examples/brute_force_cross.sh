#! /usr/bin/env bash
#./cross_validation_pr_probimage ~/src/PMC/DATA/MSRC_ObjCategImageDatabase_v2/Images/ppms/14_20_s.ppm ~/src/PMC/DATA/msrc_compressed/14_20_s.c_unary 14_20_s
img_dir=/home/amir/src/PMC/DATA/MSRC_ObjCategImageDatabase_v2/Images/ppms/ #/Users/amirrahimi/Downloads/MSRC_ObjCategImageDatabase_v2/Images/ppms/
unary_dir=/home/amir/src/PMC/DATA/msrc_compressed/ #/Users/amirrahimi/Downloads/msrc_compressed/
newgt_dir=/home/amir/src/PMC/DATA/newgt/ #/Users/amirrahimi/Downloads/newgt/

for x in `ls $newgt_dir*.bmp`; do
    a=${x:0:${#x}-7};
    filename=$(basename $a); 
    unary=$unary_dir$filename'.c_unary';
    `mkdir -p RESULTS/$filename/orig/`
    echo $filename
    ./cross_validation_pr_probimage $img_dir$filename'.ppm' $unary_dir$filename'.c_unary' $filename
done
