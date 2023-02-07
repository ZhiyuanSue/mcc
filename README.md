# Introduction

(By now, **only the front end of the mcc has been done.**

And the backend of the complier is still need working)

mcc(named by Magic C compiler) is a C compiler that designed completely support the C11 standard.

mcc not use any tools except for GNU gcc, which means no  Bison/flex LLVM,etc.

mcc is designed to bootstrap itself.

A C compiler have several stages: 

· preprocessing

· compiling

· assembling

· linking. 

mcc at least support the compiling stage.

It is guaranteed that the code except '/lib' is all written by myself . But for the files under the '/lib' is the library of standard C , it might copy from GNU C lib or other places like Microsoft. For those files , if I copy from other places, I will note it.

## Version record

2021.12——2022.6 I tried to build a lexer ,a parser, and build the types, do some semantics(but not all, I finished a lot of declaration semantics). you can see the ast and the symbol table on some test case

2022.6.7  v0.59 Migrate code to github, try to build some tools for expr semantics, rewrite some functions use those tools.

## why use C11 standard？

When I build mcc, the newest C standard version is C17, so there's a question——why I don't use that standard version?

Let's talk about those versions:

Before C11, each version add something important and (some times) useful features on the base of the previous one.

But  C17 seems did not add anything new, and it just modify or adjust something compared to C11.（I'm not very sure about it）Besides,even the Linux kernel is now have a plan to update the C version to C11——not C17 directly. So I think it's reasonable to use C11 instead of C17.

The C language is an old one .And I have to support as more as possible features of C ——otherwise it's just a tool and cannot be used at some cases.  C17 support seems not necessary——you can try to fix it in my project. 

Besides，the C version of the complier like lcc or ucc is toooo old, which means some times they can only be used as a tool. I agree the feature like 'thread local' must be support by the OS——and I guess nobody would use my complier to do such a complex thing. But I hope it can be an example in how complier deal with such features.

## why no other tools like bison/flex LLVM?

Such a topic is described in my docs about parser.

If you want a short answer, in a word, GCC and Clang don't use them, too.

As for LLVM, it's easy to use. But it's language is C++. I known there's a C interface of LLVM——Just an interface. I have to bootstrap this complier , so the code must be pure C ,that's why I don't use LLVM.

## what kind of ISA is supported

From now on, I just run it on x86_64.But as I used the IR, and translation a language from IR to assemble is easy,I think the backend is easy to change

# Usage

## build:

Just use GNU make and make it.

In the future, I might build a VS version makefile, but I don't think it's meaningful, windows changes quickly.

If you need to change some configuration,you can rewrite the makefile and ./defs/defs.h.

## use mcc to compile

Until now,there's no any arguments set, so just use

```
./bin/mcc target
```

to run your C source file

you can also use

```
./test.sh
```

to run some test case(remember, in test cases, you might see some error or warnings,for it's just used for test that)

And you can use

```
./bootstrap.sh
```

to compliy itself. At most time, I will do my best to make sure that comply itself is usable.

mcc is just a C compiler, but it doesn't include an assembler or a linker until now.

so if you want to compiler a file that can run on your computer, you have to use GAS and GCC's linker.

After mcc generate an asm file, just use gcc to build it.

for example:

```
gcc -E hello_world.c -o hello_world.i      	/*preprocess*/
./bin/mcc hello_world.i hello_world.s		/*use mcc to compile, you will get a hello_world.s file if success*/
gcc -c hello_world.s -o hello_world.o		/*assembler*/
gcc hello_world.o -lc -o  hello_world		/*linker,please use -l to link to GCC lib*/
```

Besides, if you want to use some function in standard C lib, you can use the '/lib', or just write by yourself(it's easy ,just copy from C11 standard), then use -l to link to the GCC library. The reason of doing so is just the header of GCC is not just a standard C header——it includes something like "____attribute____" or built_in keywords. 

Let me repeat it:

```
Do not directly include GCC's header file,and use -l to link
Do not directly include GCC's header file,and use -l to link
Do not directly include GCC's header file,and use -l to link
```

##  Implementation-defined Details

This implementation-defined details in mcc is in ./defs/defs.h with macro format, you can change it as your wish

· char is equal to signed char , others is the same

· complex is supported

· _Atomic is supported

## Current work with some examples(it need always change)

All my test case is under the /bin/test_src

By now I finished the front end of my complier, and you can see the result at /bin/test_out

Let I use an example to discribe the current output 

e.g. ./bin/test_src/par_test_01.c

the source code is:

```
int main(void)
{
    return 0;
}
```

Than in the ./bin/test_out/par_test_01.txt

**If you want to see the following informations, you must find some micro in ./defs/defs.h and set them**

```
_TEST_AST_	/*print ast tree*/
_TEST_SYMBOL_TABLE_	/*print symbol table*/
_TEST_IR_	/*print ir*/
```

Other micros can also used to test some other part, you can try

The size of output might be very huge,so disable them might save some out put space, it's up to you.

### AST tree

First you can see the AST tree:

```
start print ast tree
<translation_unit ns:1>
    <external_decl ns:1>
        <function_definition ns:1>
            <&declaration_spec ns:1>
                <&type_spec ns:1>
                    <KW_int:int sym level: 1 namespace:1>
            <&declarator ns:1>
                <&direct_declarator ns:1>
                    <identifier:main sym level: 1 namespace:1>
                    <(:( sym level: 1 namespace:1>
                    <&parameter_type_list ns:1>
                        <&parameter_list ns:1>
                            <&parameter_declaration ns:1>
                                <&declaration_spec ns:1>
                                    <&type_spec ns:1>
                                        <KW_void:void sym level: 2 namespace:1>
                    <):) sym level: 1 namespace:1>
            <compound_stmt ns:1>
                <left_brace:{ sym level: 2 namespace:1>
                <statement ns:1>
                    <&return_stmt ns:1>
                        <KW_return:return sym level: 2 namespace:1>
                        <&primary_expression ns:1>
                            <integer_constant:0 sym level: 2 namespace:1>
                        <;:; sym level: 2 namespace:1>
                <right_brace:} sym level: 2 namespace:1>
finish print ast tree
```

### Symbol table

And you can also see the symbol table later:

```
start print symbol table
scope type:file_scope with child 1 and namespace 0
    <symbol table>
        <symbol:main ns:1 linkage:0>
            type::< function , signed_int >
                no parameter
    scope type:function_scope with child 1 and namespace 0
        <symbol table>
        scope type:block_scope with child 0 and namespace 0
            <symbol table>
end print symbol table
```

If you have some error in the front end, you might see an error.

e.g. ./bin/test_out/expr_test_01.txt

```
error
Compile Error:(file ./bin/test_src/expr_test_01.c: line 9: token -> )
    d->db=1;
reason:	C0054:Must be a pointer to struct/union and it's member
```

It include the error place , and the error reason. Sometimes you might see a warning.

### IR

Here,I copied some elements of IR from LLVM

As you can see in the test cases, after the front end,the source will be translated into the 

in an IR , some part should be present

#### 1/ the static storage data

let's use another example: ./bin/test_src/init_test_01.c

the source code is:

```
int a[2]={1,2};
int b={1};
int c[1]={1,2};
int d=4;
struct tmpa{
    int a;
    int b;
};
struct tmpa tmp=(struct tmpa){1,2};
struct{
        int a[3],b;
    } w[]={ {1},2,3,4,{5,6} ,[3]=7,8};
struct{
    int a;
    double b;
} x={.b=2.0};
int y[4][3] = {
    1, 3, 5, 2, 4, 6, 3, 5, 7
};
int z[4][3]={
    {1,2},
    {3,4},
};
```

which tries to initialize a lot of static storage varibles.

and you will see the output:

```
<external declarations>:9 declarations in total
	.globl	a
a:
	.long	1
	.long	2
	.globl	b
b:
	.long	1
	.globl	c
c:
	.long	1
	.globl	d
d:
	.long	4
	.globl	tmp
tmp:
	.space	0
	.globl	w
w:
	.long	1
	.space	12
	.long	2
	.long	3
	.long	4
	.long	5
	.space	16
	.long	7
	.long	8
	.space	8
	.globl	x
x:
	.space	8
	.quad	9223372036854775808
	.globl	y
y:
	.long	1
	.long	3
	.long	5
	.long	2
	.long	4
	.long	6
	.long	3
	.long	5
	.long	7
	.space	12
	.globl	z
z:
	.long	1
	.long	2
	.space	4
	.long	3
	.long	4
	.space	28
```

And above is the code style under mac, so '.space' is used to represent the '.zero' under the linux.

#### 2/the function and the labels of basic blocks

for the previous case: par_test_01.c

the IR print out like:

```
<function definitions>:1 functions in total
.globl function:main
{
	<bb :main>
		<op:OP_BR :<None>:<None>:<None>>
	<bb :.func.end.1>
		<op:OP_RET :<None>:<None>:<None>>
}
```

it have two basic block:

a block named by the function name, and the function end block

and in the basic blocks are some IR codes

## License

GPLv3 or later

I promise the code except for the files under  ./lib is all written by myself. But those codes under ./lib and the asmer,linker might be gcc's or microsoft's. I have to use this license, because there licenses are GPL.

In another word, if you only use my code and don't use any other code from gcc——maybe you need a frontend of C Complier, just ignore it——although it might cannot run.

But in the future, maybe I will rewrite the lib and try to rebuild those parts. At that case, I might use no any other people's code, and the license might change.

Of course, if you fork this project before my change, or I haven't change that, you can still use it with a GPLv3 license