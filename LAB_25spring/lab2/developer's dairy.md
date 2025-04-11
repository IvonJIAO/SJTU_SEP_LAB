# Test 1
~~1、在regisvalid中 将>=修改为>~~(其实不是问题 

2、在instruction.cpp中 将needregister和needmemory的判断逻辑修改正确

# Test 2

47 tests, 33 passed, 14 failed

*[y64-ins\call.bin:]*

MY：
```ldif
  Stopped in 1 steps at PC = 0x0.  Status 'ADR', CC Z=1 S=0 O=0
  
Changes to registers:

-%rsp: 0x0000000000000000      0xfffffffffffffff8


  Changes to memory:
--- 1,5 ----
 + PC = 0x0, Invalid stack address 0xfffffffffffffff8
  Stopped in 1 steps at PC = 0x0.  Status 'ADR', CC Z=1 S=0 O=0
  Changes to registers:

  Changes to memory:
 ```
 

 ***problem:***

 
 ```0x000: 80 00 00 00 00 00 00 00 00 (call Func1)：```
 这条指令是一个调用指令，调用地址为0x000（即自身），这会导致递归调用。

***solution:***
 在执行call指令时，检查调用地址是否在当前函数的范围内，如果是，则抛出异常或返回错误。

# Test3
## 34/47

解决了call中自身递归的问题 且返回错误地址时使用的是栈移动后的地址

# Test4
##### FAILED y64-ins\pushq.bin: stdout mismatch
```ldif
Stopped in 1 steps at PC = 0x0.  Status 'ADR', CC Z=1 S=0 O=0

Changes to registers:

-%rsp: 0x0000000000000000      0xfffffffffffffff8

  Changes to memory:
  --- 1,5 ----
+PC = 0x0, Invalid stack address 0xfffffffffffffff8
  Stopped in 1 steps at PC = 0x0.  Status 'ADR', CC Z=1 S=0 O=0
  Changes to registers:

  Changes to memory:
```
***problem：***

没有判断将要压入的栈地址是否为空

***solution：***

增加了对栈地址的条件判断

# Test5
## 35/47
按照说明书，修改了popq和pushq的对象是rsp的情况
## 38/47

最基本的用例都通过了，剩下综合用例

又加了一些异常的判断

# ***写不动了，期中考试之后再debug***  :cry




---

*分割线*
___

# Test 6
##### [FAILED] y64-app\pushquestion

***solved:***

压栈的值是改变之前的rsp 但是内存地址改为了更新之后的 
# Test 7

##### [FAILED] y64-app\pushtest.bin: stdout mismatch
```ldif
*** 1,5 ****

! PC = 0xc, Invalid stack address 0xfffffffffffffff8

! Stopped in 3 steps at PC = 0xc.  Status 'ADR', CC Z=1 S=0 O=0

Changes to registers:

Changes to memory:

--- 1,7 ----

! Stopped in 6 steps at PC = 0x12.  Status 'HLT', CC Z=1 S=0 O=0

Changes to registers:

+%rdx: 0x0000000000000000      0x0000000000000100

+%rsp: 0x0000000000000000      0x0000000000000100

  Changes to memory:

+0x00000000000000f8:   0x0000000000000000      0x0000000000000100
```

**problem？**

rrmovq有问题？ irmovq有问题？

***solution:***

>*rrmovq赋值赋反了！*


TO BE SOLVED

# Test 8
## 41/47

##### [FAILED] y64-app\prog10.bin: stdout mismatch

```ldif
*** 1,7 ****
! PC = 0xc, Invalid stack address 0x1
  Stopped in 3 steps at PC = 0xc.  Status 'ADR', CC Z=1 S=0 O=0
  Changes to registers:
  %rax: 0x0000000000000000      0x0000000000000001
- %rsp: 0x0000000000000000      0xfffffffffffffff8

  Changes to memory:
--- 1,6 ----
! PC = 0xc, Invalid stack address 0xfffffffffffffff8
  Stopped in 3 steps at PC = 0xc.  Status 'ADR', CC Z=1 S=0 O=0
  Changes to registers:
  %rax: 0x0000000000000000      0x0000000000000001

  Changes to memory:
```
**problem？**

先执行地址更改还是条件判断顺序上反了

***solution:***

已更改

# Test 9
## 42/47
##### [FAILED] y64-app\j-cc.bin: stdout mismatch
```ldif
*** 1,7 ****
! Stopped in 12 steps at PC = 0x44.  Status 'HLT', CC Z=0 S=0 O=0
Changes to registers:
! %rax: 0x0000000000000000      0x0000000000000060
! %rdx: 0x0000000000000000      0x0000000000000041
%rbp: 0x0000000000000000      0x0000000000000004
%rsi: 0x0000000000000000      0x0000000000000001
%rdi: 0x0000000000000000      0x0000000000000002

--- 1,7 ----
! Stopped in 9 steps at PC = 0x3e.  Status 'HLT', CC Z=0 S=1 O=0
Changes to registers:
! %rax: 0x0000000000000000      0xffffffffffffffa0
! %rdx: 0x0000000000000000      0x0000000000000040
%rbp: 0x0000000000000000      0x0000000000000004
%rsi: 0x0000000000000000      0x0000000000000001
%rdi: 0x0000000000000000      0x0000000000000002
```

***problem：***

第9步halt的退出没有执行？

*[]还是条件跳转不正确？*

**还是计算逻辑不对？**

***solution:***

>计算的时候 ab的位置反了！！
> 
>satisfy函数的调用出现了问题 应该直接使用已存在了的cc，而不是用conditioncode()
>    ***这样会多调用一次构造函数 丢失cc***
> 
> 同时去除了压栈之前对寄存器是否为空的判断，**寄存器为0的时候也是合法的地址**


# Test 10
## 46/47
### ***最后一个bug了啊啊啊啊啊啊啊***

##### [FAILED] y64-app\abs-asum-cmov.bin: stdout mismatch
```ldif
*** 1,11 ****
  Stopped in 58 steps at PC = 0x61.  Status 'HLT', CC Z=1 S=0 O=0
  Changes to registers:

! %rax: 0x0000000000000000      0xffffffffffff6a4d
  %rcx: 0x0000000000000000      0x0000000000000040
  %rbx: 0x0000000000000000      0xffffffffffffffff
  %rsp: 0x0000000000000000      0x00000000000001f0
  %rbp: 0x0000000000000000      0x0000000000000200
! %rsi: 0x0000000000000000      0xffffffffffff6000
  %rdi: 0x0000000000000000      0x000000000000a000

  
  Changes to memory:
--- 1,11 ----
  Stopped in 58 steps at PC = 0x61.  Status 'HLT', CC Z=1 S=0 O=0
  Changes to registers:
! %rax: 0x0000000000000000  0x000000000000abcd
  %rcx: 0x0000000000000000  0x0000000000000040
  %rbx: 0x0000000000000000  0xffffffffffffffff
  %rsp: 0x0000000000000000  0x00000000000001f0
  %rbp: 0x0000000000000000  0x0000000000000200
 !%rsi: 0x0000000000000000	0x000000000000a000
  %rdi: 0x0000000000000000  0x000000000000a000

  Changes to memory:


```
