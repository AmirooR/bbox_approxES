#! /usr/bin/env bash
#run with: cat /home/amir/src/PMC/DATA/MSRC_ObjCategImageDatabase_v2/Images/Test.txt| parallel "./test_single_orig.sh {.}"

img_dir=/home/amir/src/PMC/DATA/MSRC_ObjCategImageDatabase_v2/Images/ppms/ #/Users/amirrahimi/Downloads/MSRC_ObjCategImageDatabase_v2/Images/ppms/
unary_dir=/home/amir/src/PMC/DATA/msrc_compressed/ #/Users/amirrahimi/Downloads/msrc_compressed/

filename=$1
unary=$unary_dir$filename'.c_unary';
`mkdir -p RESULTS/$filename/orig/`
echo $filename
../../dense_inference_orig_probimage $img_dir$filename'.ppm' $unary_dir$filename'.c_unary' $PWD'/RESULTS/'$filename'/orig/'$filename'.ppm' >$PWD'/RESULTS/'$filename'/orig/output.txt'

