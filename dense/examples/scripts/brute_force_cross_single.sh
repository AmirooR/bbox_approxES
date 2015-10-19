#! /usr/bin/env bash
# ./cross_validation_pr ~/src/PMC/ParametricMaxflow/Data/data_GT/227092.jpg ~/src/PMC/ParametricMaxflow/Data/boundary_GT_lasso/227092.bmp 227092 unary/227092_0.unary 
img_dir=~/Downloads/data_GT/
lasso_dir=~/Downloads/boundary_GT_lasso/
unary_dir=unary/
train_file=~/Desktop/Test25.txt

#newgt_dir=/home/amir/src/PMC/DATA/newgt/ #/Users/amirrahimi/Downloads/newgt/

while read x
do
    name="${x%.*}"
    img_file=$img_dir$x
    lasso_file=$lasso_dir$name'.bmp'
    unary_file=$unary_dir$name'_0.unary'
    if [ -f $unary_file ]; then
        echo $name
        ./cross_validation_pr $img_file $lasso_file $name $unary_file
    fi
done < $train_file

