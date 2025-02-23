### What is @number at a Function Name

In 32bit windows, compilers can provide information about the [[Practical Reverse Engineering#Calling Convention|Calling Convention]] of a certain function and the size in bytes of the arguments it gets in decimal

|Calling Convention|Decoration|
|---|---|
|**`__cdecl`**|Leading underscore (**`_`**)|
|**`__stdcall`**|Leading underscore (**`_`**) and a trailing at sign (**`@`**) followed by the number of bytes in the parameter list in decimal|
|**`__fastcall`**|Leading and trailing at signs (**`@`**) followed by a decimal number representing the number of bytes in the parameter list|
|**`__vectorcall`**|Two trailing at signs (**`@@`**) followed by a decimal number of bytes in the parameter list|
|Examples||

```c
int __cdecl function(int a, int b, int c); // In assembly _function
int __stdcall function(int a, int b, int c); // In assembly _function@12
int __fastcall function(int a, int b, int c); // In assembly @function@12
int __vectorcall function(int a, int b, int c); // In assembly function@@12
```

### Implementation of C Functions is ASM

[[simple_linux.s|strlen]] [[simple_linux.s|strchr]] [[simple_linux.s|memcpy]] [[simple_linux.s|memset]] [[simple_linux.s|strcmp]] [[simple_linux.s|strset]]
### Decompilation of Windows Functions
[[KeInitializeDpc.c|KeInitializeDpc]]
[[KeInitializeApc.c|KeInitializeApc]] 
ObFastDereferenceObject + Its Calling Convention 
KeInitializeQueue 
KeWaitForLockChainValid 
KeReadyThread 
KiInitializeTSS 
RtlValidateUnicodeString
### Recover Functions From Sample H

#### SUB_10BB6

#### SUB_11732

### Recover SUB_1000CEA0 From Sample L

### Why Can't User Modify CS to Change Current Privilege Level

### Read and Perform Virtual to Physical Translation

### How Data Execution Prevention Works