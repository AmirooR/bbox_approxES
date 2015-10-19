#! /usr/bin/env bash
my_func()
{
    ./stereo-orig $1 $2 $3 $4 $5 $6
}
#tsukuba
img_left=/media/amir/Data/BACKUP/src/PMC/DATA/Stereo/tsukuba/scene1.row3.col3.ppm
img_right=/media/amir/Data/BACKUP/src/PMC/DATA/Stereo/tsukuba/scene1.row3.col4.ppm
num_disp=16
scale=16
export -f my_func

parallel my_func $img_left $img_right $PWD'/lambda_loop/tsukuba2/' $num_disp $scale ::: $(seq 0.0 0.05 15.1)

#./stereo-orig /media/amir/Data/BACKUP/src/PMC/DATA/Stereo/tsukuba/scene1.row3.col3.ppm /media/amir/Data/BACKUP/src/PMC/DATA/Stereo/tsukuba/scene1.row3.col4.ppm lambda_loop/tsukuba/ 16 16 5.0
