#! /usr/bin/env bash
#run with: cat ~/Desktop/Test25.txt| parallel "./create_masks_single.sh {}"

filename_ext=$1
filename="${filename_ext%.*}"
img_file='/home/amir/Downloads/data_GT/'$filename_ext;
echo $filename
#in_dir='RESULTS/'$filename'/l_-0.5_0.5/';
in_dir='/media/amir/Data/BACKUP/src/PMC/bbox_approxES/build/dense/examples/best/RESULTS_SAVED_CPMC_LASSO/NONZeroUnary_-10_10_300iter_withOrigDRF/Train/'$filename'/l_-0.5_0.5/';
./maskify_one.py $in_dir $img_file
