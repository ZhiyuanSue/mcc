#!/bin/bash
#-----------------------------------------
# Filename: test.sh
# Version:  0.19
# Author: Zhiyuan Sue
# Description:
#   This file is test all the possible error in mcc
#-----------------------------------------
mcc_path="./bin/mcc"
test_src_path="./bin/test_src/"
test_out_path="./bin/test_out/"
test_asm_path="./bin/test_asm/"
file_num=0
succ_accu=0
succ_num=0
error_num=0
echo "=========="
echo "start test"
echo "=========="
if [ ! -d ${test_out_path} ];then
    mkdir ${test_out_path}
fi
if [ ! -d ${test_asm_path} ];then
    mkdir ${test_asm_path}
fi
src_dir=$(ls ${test_src_path})
out_dir=$(ls ${test_out_path})
asm_dir=$(ls ${test_asm_path})
for file in ${out_dir}
do
    #echo "----remove "${test_out_path}${file}
    rm -rf ${test_out_path}${file}
done
for file in ${src_dir}
do
    ((file_num++))
    #echo "----start test "${file}"..."
    ${mcc_path} ${test_src_path}${file} ${test_asm_path}${file} > ${test_out_path}${file%.c*}".txt"
    last_line=$(tail -n 1 ${test_out_path}${file%.c*}".txt")
    if [[ ${last_line} == "test pass!" ]]
    then
        gcc -fno-asynchronous-unwind-tables -S  ${test_src_path}${file} -o ${test_asm_path}${file%.i*}".s"
        echo ${file_num}"    succ   "${file}"   "${succ_accu}
        ((succ_num++))
        ((succ_accu++))
    else
        echo ${file_num}"    fail   "${file}"   "${succ_accu}
        ((error_num++))
    fi
    #echo "    finish test "${file}"."
done
echo "success : "${succ_num}
echo "error : "${error_num}