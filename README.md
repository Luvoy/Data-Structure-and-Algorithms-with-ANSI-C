# Data Structure and Algorithms with ANSI-C

## About The Project
尝试使用ANSI C实现各种经典的数据结构和算法，就当复习和折腾了。

鬼知道我能坚持写到哪。囧(￣_￣|||)。

有些东西还是需要面向对象语言实现啊，比如模板。

大量用了```void*```万能指针和万能函数指针，目前可能存在问题。
## Structure
1. main文件夹中放置了main函数，调用test文件夹下的各个测试程序。
2. test文件夹下分别是各种数据结构的测试程序。
3. Vector、LinkedList、HashMap等文件夹下各自是相应数据结构的头文件和实现。

## Content Introduction
* ```my_bool.h```定义了bool类型。
* ```object.h```定义了类似于object基类的结构，包括属性和方法，属性用变量实现，方法用函数指针代替，包括：
  * 指针大小属性（一般是4）
  * 某个具体结构体或类型的总大小（```sizeof(sth)```）
  * 开辟内存空间的函数指针（实现时一般用malloc函数开辟堆内存）
  * 释放内存空间的函数指针
  * 比较的函数指针
  * 打印（输出）的函数指针，可指定输出流```FILE *```
  * 深拷贝的函数指针（这里存在问题，尚待完善）
  * 其他待补充...
* ```str_func.h```中实现了一些常用的字符串函数，比如```itoa```函数，可用于数字转字符串，又比如```long_long_int_format_align_str```函数，可用于动态决定%nd中的n以便于对齐打印（花里胡哨却并无卵用XD😆）
* Vector是一种使用万能指针的、动态分配内存、内存连续的、同一类型的数组。
* LinkedList是链表结构
  * 只实现了带头节点的单向**不**循环链表
* HashMap是key、value成对储存的结构，底层由Vector和LinkdedList组成，支持key为null，不支持重复key，存取无序。

## Acknowledgment
[Object-Oriented Programming With ANSI-C](https://link.zhihu.com/?target=http%3A//www.cs.rit.edu/~ats/books/ooc.pdf)（虽然我下载了这本书只是吃灰，还没看XD）

欢迎大佬批评指正！（。＾▽＾）
