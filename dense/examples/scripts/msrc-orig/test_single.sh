#! /usr/bin/env bash


#run with: cat /home/amir/src/PMC/DATA/MSRC_ObjCategImageDatabase_v2/Images/Test.txt| parallel "./test_single.sh {.}"

img_dir=/home/amir/src/PMC/DATA/MSRC_ObjCategImageDatabase_v2/Images/ppms/ #/Users/amirrahimi/Downloads/MSRC_ObjCategImageDatabase_v2/Images/ppms/
unary_dir=/home/amir/src/PMC/DATA/msrc_compressed/ #/Users/amirrahimi/Downloads/msrc_compressed/
#newgt_dir=/home/amir/src/PMC/DATA/newgt/ #/Users/amirrahimi/Downloads/newgt/
test_list=/home/amir/src/PMC/DATA/MSRC_ObjCategImageDatabase_v2/Images/Test.txt

filename=$1
unary=$unary_dir$filename'.c_unary';
`mkdir -p $PWD/RESULTS/$filename/l_0.0_20/`
echo $filename
../../dense_inference_pr_probimage $img_dir$filename'.ppm' $unary_dir$filename'.c_unary' $PWD'/RESULTS/'$filename'/l_0.0_20/'>$PWD'/RESULTS/'$filename'/l_0.0_20/output.txt'

