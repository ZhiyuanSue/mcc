# name to be set
Bin:=./bin
Target:= ${Bin}/mcc
SRC:=./src
Defs:=./defs
Tools:=./tools
Front:=./frontend
IR:=./IR
Back:=./backend
Inc:= ${Defs}
Obj:=./obj
modules += src defs tools frontend IR backend
#
Source:=$(foreach n,$(modules),$(wildcard $(n)/*.c))
Object= $(patsubst %.c,$(Obj)/%.o,$(notdir $(Source)))

CC:= gcc
CFlags:= -g -Wall --std=c11 -O3 -I ${Inc} 


all: init ${Target}

init:
	$(shell if [ ! -d $(Obj) ];then mkdir $(Obj); fi)

${Target}:$(Object)
	${CC} $(CFlags) -lc -o $@ ${Object}


${Obj}/%.o: $(SRC)/%.c
	$(CC) $(CFlags) -o $@ -c $<
${Obj}/%.o: $(Defs)/%.c
	$(CC) $(CFlags) -o $@ -c $<
${Obj}/%.o: $(Tools)/%.c
	$(CC) $(CFlags) -o $@ -c $<
${Obj}/%.o: $(Front)/%.c
	$(CC) $(CFlags) -o $@ -c $<
${Obj}/%.o: $(IR)/%.c
	$(CC) $(CFlags) -o $@ -c $<
${Obj}/%.o: $(Back)/%.c
	$(CC) $(CFlags) -o $@ -c $<
.PHONY:clean

clean:
	rm -f ${Obj}/*.o 
	rm -f ${Target}

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