#! /usr/bin/env bash


#run with: cat Test.txt| parallel "./test_single.sh {}"
#img_dir=~/src/PMC/ParametricMaxflow/Data/data_GT/
#lasso_dir=~/src/PMC/ParametricMaxflow/Data/boundary_GT_lasso/
unary_dir=unary/
img_dir=~/Downloads/data_GT/
lasso_dir=~/Downloads/boundary_GT_lasso/

filename=$1
name="${filename%.*}"
unary=$unary_dir$name'_0.unary';
`mkdir -p $PWD/lambda_loop/$name/`
echo $filename
# multiplicative lasso 
#./dense_inference_pr $img_dir$filename $unary $PWD'/RESULTS/'$name'/l_0.5_100/'>$PWD'/RESULTS/'$name'/l_0.5_100/output.txt'

# cpmc lasso
for fp in $(seq -10.0 .1 10.0)
do
    ./dense_inference_pr_lasso_cpmc_orig $img_dir$filename $unary $lasso_dir$name'.bmp' $PWD'/lambda_loop/'$name'/' $fp
done

# additive lasso
#./dense_inference_pr_lasso_additive $img_dir$filename $unary $PWD'/RESULTS/'$name'/l_0.0_100/'>$PWD'/RESULTS/'$name'/l_0.0_100/output.txt'

