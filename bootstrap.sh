#!/bin/bash
#-----------------------------------------
# Filename: test.sh
# Version:  0.70
# Author: Zhiyuan Sue
# Description:
#   This file is used to bootstrap,but I need a preprocessor first
#-----------------------------------------
mcc_path="./bin/mcc"
bootstrap_src_path="./pre/"
bootstrap_print_out_path="./bin/bootstrap_print_out/"    #used to store the print information
bootstrap_asm_out_path="./bin/bootstrap_asm_out/"        #used to store the output asm file
file_num=0
succ_accu=0
succ_num=0
error_num=0
echo "=========="
echo "start bootstrap"
echo "=========="
if [ ! -d ${bootstrap_print_out_path} ];then
    mkdir ${bootstrap_print_out_path}
fi
if [ ! -d ${bootstrap_asm_out_path} ];then
    mkdir ${bootstrap_asm_out_path}
fi
for file in $(ls ${bootstrap_print_out_path})
do
    rm -rf ${bootstrap_print_out_path}${file}
done
for file in $(ls ${bootstrap_asm_out_path})
do
    rm -rf ${bootstrap_asm_out_path}${file}
done

for file in $(ls ${bootstrap_src_path})
do
    ((file_num++))
    ${mcc_path} ${bootstrap_src_path}${file} ${bootstrap_asm_out_path}${file} > ${bootstrap_print_out_path}${file%.i*}".txt"
    last_line=$(tail -n 1 ${bootstrap_print_out_path}${file%.i*}".txt")
    if [[ ${last_line} == "test pass!" ]]
    then
        echo ${file_num}"    succ   "${succ_accu}   "${file}"
        ((succ_num++))
        ((succ_accu++))
    else
        echo ${file_num}"    fail   "${succ_accu}   "${file}"
        ((error_num++))
    fi
done
echo "success : "${succ_num}
echo "error : "${error_num}