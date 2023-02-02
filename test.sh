#!/bin/bash
#-----------------------------------------
# Filename: test.sh
# Version:  0.19
# Author: Zhiyuan Sue
# Description:
#   This file is test all the possible error in mcc
#   If your gcc give some warning or error,emmm, it's normal.
#   Just because I use some grammar which might be legal in ANSI C but will cause some warnings in gcc
#-----------------------------------------
mcc_path="./bin/mcc"
test_src_path="./bin/test_src/"
test_out_path="./bin/test_out/"
test_asm_path="./bin/test_asm/"
test_asm_gcc_path="./bin/test_asm/gcc/"
test_asm_mcc_path="./bin/test_asm/mcc/"
bit32=0
bit64=1
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
if [ ! -d ${test_asm_gcc_path} ];then
    mkdir ${test_asm_gcc_path}
fi
if [ ! -d ${test_asm_mcc_path} ];then
    mkdir ${test_asm_mcc_path}
fi
src_dir=$(ls ${test_src_path})
out_dir=$(ls ${test_out_path})
asm_gcc_dir=$(ls ${test_asm_gcc_path})
asm_mcc_dir=$(ls ${test_asm_mcc_path})
for file in ${out_dir}
do
    rm -rf ${test_out_path}${file}
done
for file in ${asm_gcc_dir}
do
    rm -rf ${test_asm_gcc_path}${file}
done
for file in ${asm_mcc_dir}
do
    rm -rf ${test_asm_mcc_path}${file}
done
for file in ${src_dir}
do
    ((file_num++))
    #echo "----start test "${file}"..."
    ${mcc_path} ${test_src_path}${file} ${test_asm_mcc_path}${file%.c*}".s" > ${test_out_path}${file%.c*}".txt"
    last_line=$(tail -n 1 ${test_out_path}${file%.c*}".txt")
    if [[ ${last_line} == "test pass!" ]]
    then
        if [ "${bit64}" == 1 ]
        then
            gcc -fno-asynchronous-unwind-tables -S  ${test_src_path}${file} -o ${test_asm_gcc_path}${file%.c*}".s"
        fi
        if [ "${bit32}" == 1 ]
        then
            gcc -m32 -fno-asynchronous-unwind-tables -S  ${test_src_path}${file} -o ${test_asm_gcc_path}${file%.c*}".s"
        fi
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