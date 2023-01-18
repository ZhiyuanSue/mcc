# name to be set
include ./Makefile.env
export MCC_ROOT_DIR=$(shell pwd)
export MCC_ROOT_OBJ=${MCC_ROOT_DIR}/obj
export MCC_ROOT_PRE=${MCC_ROOT_DIR}/pre
export MCC_ROOT_ASM=${MCC_ROOT_DIR}/asm
export MCC_BOOTSTRAP_OUT=${MCC_ROOT_DIR}/bin/bootstrap_print_out
Bin:=${MCC_ROOT_DIR}/bin
Target:= ${Bin}/mcc
modules += src backend IR frontend tools defs

all: init SubModules  ${Target}

init:
	$(shell if [ ! -d $(MCC_ROOT_OBJ) ];then mkdir $(MCC_ROOT_OBJ); fi)
	$(shell if [ ! -d $(MCC_ROOT_PRE) ];then mkdir $(MCC_ROOT_PRE); fi)
	$(shell if [ ! -d $(MCC_ROOT_ASM) ];then mkdir $(MCC_ROOT_ASM); fi)
SubModules:
	@$(foreach n,$(modules),$(call build_submodule,$(n)))
${Target}:
	${CC} $(CFlags) -lc -o $@ $(wildcard ${MCC_ROOT_OBJ}/*.o)

.PHONY:clean

clean:
	rm -f ${MCC_ROOT_OBJ}/*.o 
	rm -f ${Target}
	rm -f ${Bin}/test_out/*
	rm -f ${MCC_ROOT_PRE}/*
	rm -f ${MCC_BOOTSTRAP_OUT}/*
	rm -f ${MCC_ROOT_ASM}/*
	rm -f ${Bin}/test_asm/*

clean_obj:
	rm -rf ${MCC_ROOT_OBJ}

#wildcard 通配符，例如：$(wildcard $(DIR)/*.c)表示某个目录下所有的c文件
#parsubst 用于替换的，例如要编译所有的c变成.o文件，那这.o文件的名称肯定需要生成，例如
# $(patsubst %.c,$(OBJ)/%.o,$(notdir $(SOURCE)))
#notdir 去除所有路径
# *.c 和 %.c的区别，前者基本用于系统中，后者只用于本makefile文件
# $@表示目标文件
# $^所有依赖文件
# $<第一个依赖文件
#关于.PHONY
#这个问题比较复杂，通常来说，用于.PHONY: clean然后让clean强制进行执行一下
#目的是防止有一个clean文件，而clean文件所依赖的文件是最新的话，那么这个规则将被视为编译clean文件这件事
#然而实际上，这只是一个通常的用法而已，应该是说后面的条件总是不满足，而必须执行一次
#所以如果有需要，每次都强制更新所有规则让他进行更新也是可以的
#函数
#call语句
#$(call fun,param1,param2,...)
#例如
#reverse = $(2)$(1)
#foo = $(call reverse,a,b)
#all:
# @echo "foo=$(foo)"
#最后显示foo=ab
#filter：代表中目标串中找出符合匹配规则的。
#filter-out：代表从目标串中过滤掉符合匹配规则的。
#@ 用于关闭命令的回显