#!/bin/bash
#-----------------------------------------
# Filename: test.sh
# Version:  0.70
# Author: Zhiyuan Sue
# Description:
#   This file is used to bootstrap,but I need a preprocessor first
#   TODO:This file haven't been done
#-----------------------------------------
mcc_path="./bin/mcc"
test_out_path="./bin/bootstrap_out/"
file_num=0
succ_accu=0
succ_num=0
error_num=0
echo "=========="
echo "start bootstrap"
echo "=========="
out_dir=$(ls ${test_out_path})
for file in ${out_dir}
do
    rm -rf ${test_out_path}${file}
done

echo "success : "${succ_num}
echo "error : "${error_num}