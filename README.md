# Introduction

**It is not usable now! I'm still writing the codes.**

**It is not usable now! I'm still writing the codes.**

**It is not usable now! I'm still writing the codes.**

(I think nobody will use it even after I finish it )



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

In the future, I might build a VS version makefile, but I don't think it's meaningful, windows changes quietly.

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

to run some test case

mcc is just a C compiler, but it doesn't include an assembler or a linker until now.

so if you want to compiler a file that can run on your computer, you have to use GAS and GCC's linker.

After mcc generate an asm file, just use gcc to build it.

for example:

```
gcc -E hello_world.c -o hello_world.i      	/*preprocess*/
./bin/mcc hello_world.i						/*use mcc to compile, you will get a hello_world.s file if success*/
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



## use mcc to bootstrap



##  Implementation-defined Details

This implementation-defined details in mcc is in ./defs/defs.h with macro format, you can change it as your wish

· char is equal to signed char , others is the same

· complex is supported

· _Atomic is supported

## License

GPLv3 or later

I promise the code except for the files under  ./lib is all written by myself. But those codes under ./lib and the asmer,linker might be gcc's or microsoft's. I have to use this license, because there licenses are GPL.

In another word, if you only use my code and don't use any other code from gcc——maybe you need a frontend of C Complier, just ignore it——although it might cannot run.

But in the future, maybe I will rewrite the lib and try to rebuild those parts. At that case, I might use no any other people's code, and the license might change.

Of course, if you fork this project before my change, or I haven't change that, you can still use it with a GPLv3 license