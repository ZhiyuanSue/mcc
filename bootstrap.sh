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
test_print_out_path="./bin/bootstrap_print_out/"    #used to store the print information
test_asm_out_path="./bin/bootstrap_asm_out/"        #used to store the output asm file
file_num=0
succ_accu=0
succ_num=0
error_num=0
echo "=========="
echo "start bootstrap"
echo "=========="
out_dir=$(ls ${test_print_out_path})
for file in ${out_dir}
do
    rm -rf ${test_out_path}${file}
done

echo "success : "${succ_num}
echo "error : "${error_num}