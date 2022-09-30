## bugs to fix:

### · parser

这里只列举已知的问题。

· semantic

常量表达式里面-1不对，unary节点的问题。

内存池不对(放弃了)

## todo list

### · args参数处理

实现

### · 自动测试脚本

实现（done）

需要实现多层次的makefile，在目前仍然是单层的。（done）

bootstrap

### · preprocess

实现

（我必须在这里指出，GNU C添加了一些东西，比如__attribute__关键字等等。我真的想摔桌子，我再用GCC我就是伞兵！尼玛预编译引入了一大堆东西，标准库都用不了，难道我得自己实现一个标准库不成）

### · lex

错误处理（done）

测试样例

### · parser

错误处理（done）

测试样例

作用域和命名空间（done）

typedef name链表的实现和overload（done）

功能迭代优化（done）

​	· 加入记忆（done）

​	· 递归转循环（可做）（不做了，没那个水平）

构建完语法树之后，进行数据结构的填充，树的折叠等等（语义分析任务，这阶段不做了）

预编译文件格式的设计和读取。（暂缓）

### · mem

实现（done）

后来发现一堆问题，扔掉了，用系统的内存池吧。

### · ASM

加了内联汇编的内容

词法（done）

语法（done）

语义分析

翻译中间代码设计

直接用内联汇编调用系统调用（不用了，链接c库就好了）

### · semantic

查看C标准的语义定义

制定各种node的属性，类型，值等等

制定符号表符号的属性

设计报错机制（设计了，貌似卵用没啥）

根据C标准的语义定义进行语义分析（主要是类型检查）(done)

设计类型机制，声明（框架搭起来了，但是有些赋值与表达式有关，另外，array和function还未做）(done)，表达式（常量表达式做了），语句（做了）

linkage（做了）

Declaration模块

​	restrict

​	function specifier

​	alignment specifier(做完了，并且依然觉得这玩意又臭又长)

​	initialization(优先，毕竟其他用的人不多，但是这个用的很多)

​	staticassertion

expression模块（基本完工）

因为改了常量表达式的表达方式，所以enum那边出了一些问题，需要改进(done) enum真讨厌，我实在是醉了。。。


Atomic部分，对于memory_order_seq_cst内存序上面的扩展(做不来，搞不懂)

statement模块的语义分析（做完了，挺简单的）

enum类型使用一个不完整的类型的情况（可能需要读文献来判断是否可以）

compatible 类型和composite类型

### · gencode

查看LLVM

设计中间表示的数据结构

设计翻译代码方案（目前貌似会和语义分析混合在一起）
