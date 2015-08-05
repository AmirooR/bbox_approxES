#! /usr/bin/env bash
#run with: cat /home/amir/src/PMC/DATA/MSRC_ObjCategImageDatabase_v2/Images/Test.txt| parallel "./get_best_single.sh {.}"

filename=$1
echo $filename
in_dir='RESULTS/'$filename'/l_0.0_20/';
./get_best.py $in_dir
