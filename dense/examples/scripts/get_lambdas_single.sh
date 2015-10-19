#! /usr/bin/env bash 

#run with: cat /home/amir/src/PMC/DATA/MSRC_ObjCategImageDatabase_v2/Images/Test.txt| parallel "./get_lambdas_single.sh {.}"

filename=$1
#echo $filename
in_dir='RESULTS/'$filename'/l_-0.5_0.5/';
./get_best_lambdas_no_unary.py $in_dir
