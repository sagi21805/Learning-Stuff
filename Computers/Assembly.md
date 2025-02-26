_Learned via [[Practical Reverse Engineering.pdf#page=28|Practical Reverse Engineering]]_
# x86
- This is a little-endian architecture based on the Intel 8086 processor.
- This processor can operate in two mode _Real_ and _Protected_
<u>Real</u> - The processor state when it is first powered and support 16-bit instruction set
<u>Protected</u> - The processor state supporting virtual memory, paging, and other features.
This architecture supports multiple rings (0-3) for privileged, and most commonly uses ring 3 for the user and ring 0 to the kernel, this is controlled with the _cs_ register and called _CPL_ (current privilege level)
## Registers and Data Types
When operating in protected mode the system has eight 32-bit general purpose registers (GPRs): _EAX, EBX, ECX, EDX, EDI, ESI, EBP and ESP_
The instruction pointer in stored in the _EIP_ register
![[Pasted image 20250107202413.png]]

| Register | Purpose                                 |
| -------- | --------------------------------------- |
| _ECX_    | Counter in loops                        |
| _ESI_    | Source in String/Memory operations      |
| _EDI_    | Destination in String/Memory operations |
| _EBP_    | Base frame Pointer                      |
| _ESP_    | Stack Pointer                           |
### <u>Common Data Types</u>
- **Bytes** - 8-bit. Examples: _AL_, _BL_, _CL_
- **Word** - 16-bit. Examples: _AX, BX, CX_
- **Double Word** - 32-bit. Examples: _EAX, EBX, ECX_
- **Quad Word** - 64-bit. While x86 does not have 64-bit because of it's 32-bit nature, there are some instructions that return a 64-bit value, these instructions in a 32-bit environment return their output in 2 registers for example the _RDTSC_ instruction writes it's output to _EDX:EAX_
The 32-bit _EFLAGS_ register is used to store the status of arithmetic operations and other executions states (e.g., trap flag). For example, if an add operation resulted in a zero the, _ZF_ flag will be set to 1. The flags in the _EFLAGS_ register are used primarily to implement conditional branching.
In addition there are also more registers that control important low-level system mechanisms such as virtual memory, interrupts, and debugging. For example, _CR0_ controls whether paging is on or off, _CR2_ contains the linear address that caused a page fault, _CR3_ is the base address of a paging data structure, and _CR4_ controls the hardware virtualization settings. _DR0-DR7_ are used to set memory breakpoints. 
#note 
_Although there are seven debug registers the system allows only 4 memory breakpoints (DR0-DR3), and the rest are used for status.
There are also Model specific registers that will be discussed throughout the book_  
## <u>Instruction Set</u>
The x86 instruction set allows a high level of flexibility in terms of data movement between registers and memory. The movement is classified into five general methods:
1. Immediate to register
2. Register to register
3. Immediate to memory
4. Register to to memory and vice versa
5. Memory to memory.

The first 4 methods are supported by all modern architectures, but the last on is specific to x86. 
A classical RISC (Reduced Instruction Set Computer) architecture like ARM can _only_ read/write data from/to memory with load/store instructions (_LDR_ and _STR_, respectively); 
For example a simple operation like incrementing a value in memory requires three instructions:
1. Read the data from memory to register (_LDR_)
2. Add one to the register (_ADD_)
3. Write the register to memory (_STR_)
On x86 this procedure would require only one instruction _INC_ or _ADD_ which can access the memory directly. The _MOVS_ instruction can read and write to memory at the same time.

<u>Arm Assembly</u>
```arm-asm
LDR  R3, [R3]       
; Read the value at address R3

ADDS R2,  R3, #1    
; Add one to it

STR  R2, [R3]       
; Write updated value back to address R3
```
<u>x86 Assembly</u>
```nasm
inc dword ptr [eax] 
; Directly increment value at address EAX
```

Another important characteristic is that x86 uses variable-length instructions size: The instruction length can range from 1 to 15 bytes. On ARM, instructions are either 2 or 4 bytes in length.
### <u>Data Movement</u>
#### MOV Instruction
Instructions operate on values that come from registers or main memory, The most commonUSB 3.0 and 3.1 instruction for moving data is _MOV_. The simplest usage is to move a register or immediate to a register. For example:
```nasm
mov esi, 0F003Fh 
; Set ESI = 0xF003 (h for hex)

mov esi, ecx     
; Set ESI = ECX 
```
The next common usage is to move data to/from memory. Similar to other assembly conventions, x86 uses square brackets ([]) to indicate memory access. (The only exception is the _LEA_ instruction, which uses [] but does not actually reference memory.) 
<u>Simplest use case:</u>
```nasm
mov dword ptr [eax], 1 
; Set the memory at address EAX to 1

mov ecx, [eax]         
; Set ECX to the value at address [EAX]

mov [eax], ebx
; Set the memory at address EAX to EBX

mov [esi+34h], eax    
; Set the memory address at ESI+0x34 to EAX 

mov eax, [esi+34h]
; Set the EAX to the value at address esi+34h

mov edx, [ecx+eax]
; Set EDX to the value at memory address of ECX+EAX
```
Matching Pseudo C
```C
*eax = 1; 
ecx = *eax;
*eax = ebx;
*(esi+0x34) = eax;
eax = *(esi+0x34);
edx = *(ecx + eax);
```
These examples demonstrate memory access through a base register and offset, where offset can be a register or immediate. This form is commonly used to access structure members or data buffers at a location computed at runtime. For example suppose that _ECX_ points to a structure of type _KDPC_ with the layout:
```C
typedef struct {                //Memory Address
	uint8_t type,               //0x000 
	uint8_t Importance,         //0x001
	uint16_t Number,            //0x002
	_LIST_ENTRY DpcListEntery,  //0x004
	void* DeferredRoutine,      //0x00c 
	void* DeferredContext,      //0x010
	void* SystemArgument1,      //0x014
	void* SystemArgument2,      //0x018
	void* DpcData,              //0x01c
} KDCP;
```
The use of the following struct could be something like
```C
KDPC *p = ...;
p->DpcData = NULL;
p->DeferredRoutine = ...;
*(int*)p = 0x113;
p->DeferredContext = ...;
```
In assembly (Struct is located at address ECX)
```nasm
mov eax, [ebp]
; Read value from memory address EBP and store it to EAX

and dword ptr [ecx+1Ch], 0 
; And gates address at ecx+0x1C (Offset to DpcData) with 0 
; (Effictive set DcpData to 0)

mov [ecx+0Ch], eax
; Move value at EAX to address ECX+0x0C (offset to DeferredRoutine)

mov dword ptr [ecx], 113h
; Move value 0x113 to the ecx address, and enforce it to be a 32-bit address 
; (Because 0x113 is a 16-bit value the address will try to only change the first
; 16 bits but we want to change a 32bit address so we add dword ptr)  

mov [ecx+10h], eax
; Change the value at ECX+0x10 (Which is DefferedContext) to the value at EAX
```
On line 4, we write the double-word value 0x113 to the base of the structure. This is writing double-word value into a byte, which will overwrite the values after it. 
The code at line 4 could also be written less efficiently like this:
```nasm
mov byte ptr [ecx], 13h
mov byte ptr [ecx+1], 1
mov word ptr [ecx+2], 0
```
The compiler decided to fold three instructions into one because it knew the constants ahead of time, and wanted to save space. 
#### <u>Array Access</u>
Array access is most of the time accessed in the general way
array[base + index * size_of_element]

```nasm
mov esi, _KdLogBuffer[esi*4] 
; _KdLogBuffer is the base address 
; ESI in this case is the index, and the elements size is 4
```
This can be observed in code looping over an array
```nasm
loop_start:
	mov eax, [edi+4]
	; mov what is in address edi+4 to eax
	; edi is probably an address in memory
	; This can be infered because in the next line it 
	; looks like indexing an array of ints
	
	mov eax [eax+ebx*4]
	; load the ebx index of the array into eax
	
	test eax, eax
	; this does eax AND eax, and sets the ZF flag
	; Sets the ZF flag if eax == 0.
	
	jz short loc_7F627F
	; jumps into location in memory if the ZF flag is 1 
	; this means that if eax is 0, the code will jump
	; because of the last line.
	; if eax != 0 the code will continue

	..... some code .....
	
loc_7F627F:
	inc ebx
	; Add one to the ebx register
	; compare ebx, with whats inside the address at edi 
	
	cmp ebx, [edi]
	; calculates [edi] - ebx and sets the SF flag

	jl short loop_start
	; if [edi] is bigger then ebx, SF = 1
	; because the result must be negative
	; if the reuslt is negative jump to loop_start  
```

```clike
loop_start {
	eax = *(edi + 4);
	eax = *(eax + ebx * 4);
	if eax == 0 {
		jump loc_7F627F
	}
}
loc_7F627F {
	ebx+=1;
	if (*edi - ebx) < 0 {
		jump loop_start
	}
}
```

This Pseudo code can be translated to this C code
```C
typedef struct FOO {
	int size;
	int* elements;
} FOO;

FOO* edi = ...;

for (ebx = ..; ebx < edi->size; ebx++ ) { // loc_7F627F
	if (eax != 0) {
		.... some code ....
	}
	// if the code reached here this means that eax == 0
	// so there is a jump to the to the for-loop line
}

```
#### <u>LEA Instruction</u>
Load Effective address instruction loads the address of something. 
for example, if eax is an address in memory, and we want to get address eax + ebx * 3
```nasm
mov ecx, eax + ebx * 3   
; This is not valid 

mov ecx, [eax + ebx * 3] 
; This will return what is inside that address

lea ecx, [eax + ebx * 3] 
; This will load the effective address, like wanted
```
#### <u>REP Instruction</u>
Repeats an instruction the number in _ecx_ times 
for example 
```nasm
mov ecx, 10
rep scasb 
```
Will repeat the scasb command 10 times
#### <u>REPNE Instruction</u>
This instruction repeats while ZF and or ecx is not zero, this instruction also decrements ecx first, while doing so and can be see like this.
```C
while (1) {
	ecx--;
	if (ZF == 1) || (ecx == 0) {break}
}
```
	#### <u>MOVS Instruction</u>
These instructions move data with 1, 2 or 4  granularity between two memory addresses. They implicitly use the _EDI/ESI_ as the destination/source addresses respectively. In addition they also update the source and destination addresses automatically. If the DF (direction flat) is 0 addresses are decremented otherwise incremented. Typically these instructions are used to copy memory. In some cases they are coming with the _REP_ prefix which says they will repeat _ECX_ times.
```nasm
mov esi, offset _RamdiskBootGuid
; ESI = pointer to RamdiskBootGuid

lea edi, [ebp-0C0h]
; EDI is an address somewhere on the stack 

movsd 
; copies 4 bytes from ESI to EDI
; increments each by 4 
movsd
movsd
movsd
```
<u>Matching C code</u>
```C
typedef struct GUID {
	...
} GUID;

GUID RamDiskBootDiskGuid = ... ;
GUID foo;
memcpy(&foo, &RamDiskBootDiskGuid, sizeof(GUID))
```
<u>With rep</u>
```nasm
push 8
; push 8 into the stack (Discussed Later).

pop ecx 
; pop the stack, basically sets ecx to 8.

mov esi, offset _KeServiceDescriptorTable
mov edi, offset _KeServiceDescriptorTableShadow
; set source and destination addresses

rep movsd
; repeates ecx times the movsd instruction
; in this case ecx is 8 so repeates 8 times.
; this means 32 bytes are copied, so we can infer 
; that is the size of the object.

```
<u>Matching C Code</u>
```C 
typedef struct _KeServiceDescriptorTables {
	... fields ...
} KeServiceDescriptorTable; // Size 32bytes

KeServiceDescriptorTable src = ... ;
KeServiceDescriptorTable dst = ... ;

memcpy(&dst, &src, 32);
// 32 == sizeof(_KeServiceDescriptorTable) 
```

Sometimes the size is not a multiply of 4 and requires using movsb or movsw, to move a word or byte instead of a double-word
```nasm
lea esi, [eax+170h]
; eax is probably a struct in memory 

lea edi [ebx+170h] 
; ebx is probably address to structure of the same type 
; eax+170h and ebx+170h is probably another struct

movsd
movsd
movsd
movsw
; moves a word from esi to edi
movsb
; moves a bytes from esi to edi
```
This code copies 15 bytes from eax+170h to ebx+170h, we can infer because of the copy and the identical offset that they are the same struct.
#note 
This code could also be written like this 
```nasm
lea esi [eax+170h]
lea edi [ebx+170h]
mov ecx 0Eh
rep movsb
```
but this is inefficient, because it makes 15 reads instead of just 5. 
#### SCAS (Scan String) and STOS (Store String)
This is another data movement instructions with implicit source and destination, and they can also operate on 1, 2 and 4 bytes.
_SCAS_ is used to compare the _AL_/_AX_/_EAX_  registers with data at memory address _EDI_, and it automatically increments/decrements _EDI_ depending on _DF_ flag. _SCAS_ is commonly used with _REP_ to find a byte, word or double-word inside a buffer.
For example C _strlen()_ function can be implemented like that ->
```nasm
xor al, al
; Sets AL to 0 (NULL), Faster then mov reg, 0

mov ebx, edi 
; save the orignal location of the buffer

repne scasb
; repeatedly compare the next byte on edi to al
; as long as al doesn't match that byte.
; which means the string continues

sub edi, ebx
; now edi points to the end of the string
; ebx is saved as the start of the string
; so edi - ebx sets edi to the length of the string
```
 _STOS_ is the same as _SCAS_ except it write the value _AL_/_AX_/_EAX_ to _EDI_. It is commonly used to initialize a buffer to a constant value (such as _memset()_)

```nasm
xor eax, eax
; set eax to 0

push 9 
pop ecx
; set ecx to 9

mov edi, esi
; set edi to the same address as esi

rep stosd
; write ecx * double-word bytes which in this case 36 bytes into edi
```

This is equivalent to ->
```C
memset(edi, 0, 36)
```
#### LODS Instruction
Another instruction from the same family. It reads a 1-, 2-, or 4- byte value from _ESI_ and stores it in _AL_/_AX_/_EAX_

### <u>Exercise</u>
Explain what is the type of _[EBP + 8]_ and _[EBP+C]_ in line 1 and 8, respectively
next, explain what this snippet does.
1) Type of _[ebp+8]_ == char*
2) Type of [ebp+0Ch] == char

```nasm
mov edi, [ebp+8]
; set edi to the value at address ebp+8
; probably a memory address

mov edx, edi
; save the value of edi in edx

xor eax, eax
; zero out eax 

or ecx, 0FFFFFFFFh
; set ecx to FFFFFFFF (which is max int32)

repne scasb
; compare al to the values at address edi, until it equals
; probably means to search the end of a string (char*)
; while decrementing the ecx register by 1 each iteration
; after this ecx = ecx - len(edi)
; the length is including the \0 bit
; means len('123\0') is 4

add ecx, 2
; assuming the length of the string is more then two
; set the value of ecx to 0xFFFFFFFF - len(edi) + 2

neg ecx
; set ecx to it's 2s compliment, which is not ecx, add ecx, 1
; this sets ecx to the true length of the string
; which is all the charaters without the \0
 
mov al, [ebp+0Ch]
; moves a constant value to the al register, probably a char

mov edi, edx
; set edi to the memory address in edx
; which restest edi to the start of the string

rep stosb
; repeat ecx times, which is the length of the string 
; and store in each iteration the constant value al

mov eax, edx
; transfer the base of the string to eax
```
This program gets a cstring in [ebp+8] and a constant in [ebp+C], it calculates the length of the string and replaces it by the constant.
### Arithmetic Operations
These are fundamental mathematical operations such as addition, subtraction, multiplication, and division which are natively supported by the instruction set. Bit-level operations such as _AND_, _OR_, _XOR_, _NOT_ and left and right shift operations also have native corresponding instructions.  With the exception of multiplication and division, the remaining instructions are straightforward. in terms of usage
```nasm
add esp, 14h         ; esp = esp + 0x14
sub ecx, eax         ; ecx = ecx - eax
sub esp, 0Ch         ; esp = esp - 0xC
inc ecx              ; ecx = ecx + 1
dec edi              ; edi = edi - 1
or eax, 0FFFFFFFFh   ; eax = eax | 0xFFFFFFFF
and ecx, 7           ; ecx = ecx & 7
xor eax, eax         ; eax = eax ^ eax
not edi              ; edi = ~edi
shl cl, 4            ; cl = cl << 4
shr ecx, 1           ; ecx = ecx >> 1
rol al, 3            ; rotate AL left 3 positions
ror al, 1            ; rotate AL right 1 position 
```

The _Left_ and _Right_ shift operations are used frequently as _Strength Reduction_ because they replace computationally intensive operations such as multiplication or division. 
When the divisor is know to be a power of 2, it can always be reduced to bit shifting, for example, 100 * 2 is the same as 100 <<1 and 100/2 is the same as 100>>1 which are much much faster operations.

#### MUL Instruction
This is a multiplication instruction, that has the following general form. _mul reg/memory_ 
The given register is multiplied with _AL_, _AX_, or _EAX_, The result is stored at _AX_, _DX_:_AX_ or 
_EDX_:_EAX_ depending on the operand width.
```nasm
mul ecx                ; EDX:EAX = EAX * ECX
mul dword ptr [esi+4]  ; EDX:EAX = EAX * dword_at(ESI+4)
mul cl                 ; AX = AL * CL
mul dx                 ; DX:AX = AX * DX
```
For example: 
```nasm
mov eax, 3 
mov ecx, 22222222h
mul ecx
; EDX:EAX = 3 * 0x22222222 = 0x6666666 (no overflow) EDX = 0,
; EAX = 0x66666666

mov eax, 3
mov ecx, 80000000h
mul ecx 
; EDX:EAX = 3 * 0x80000000 = 0x180000000 (overflow) EAX = 1,
; EAX = 0x800000000
```
<u>IMUL Instruction</u>
Very similar to MUL
- _IMUL_ reg/mem -> same as mul (multiplied by EAX)
- _IMUL_ reg1, reg2/mem -> reg1 = reg1 * reg2/mem
- _IMUL_ reg1, reg2/mem, imm -> reg1 = reg2/mem * imm
#### DIV Instruction
This is a division instruction that take the divisor as parameter and have the following form: 
_DIV_ reg/mem. Depending on the divisor size, DIV will use _AX_, _DX_:_AX_ or _EDX_:_EAX_. the resulting quotient/reminder pair are stored in _AL_/_AH_ _AX_/_DX_ or _EAX_/_EDX_
```nasm
div ecx                  ; EDX:EAX / ECX quotient in EAX
div cl                   ; AX / CL, quocient in AL, reminder in AH
div dword ptr [esi+24h]  ; same as line one 

mov cl, 2
mov eax, 0Ah
div cl 
; AX / CL = A/2 = 5 in AL (quotient) AH = 0 (reminder)

mov cl, 2
mov eax, 09h
div cl
; AX / CL = 9/2 = 4 in AL, AH = 1 (reminder)
```

### Stack Operations and Function Invocations
The stack is a fundamental data structure in programming languages and operating systems. For example, local variables in C are stored on the functions stack space. 
When operating systems transition from ring 3 to ring 0, it saves state information on the stack. 
#### PUSH Instruction
This instruction decrements the _ESP_ (Stack Pointer) <u>and then</u> writes data in the location in memory pointed by _ESP_ 
#### POP Instruction
This instruction increments the _ESP_ register <u>and then</u> reads data from the location in memory pointed by _ESP_. This instruction also takes a register to write the data to, and can read 1, 2 or 4 bytes with a prefix override. In practice the default value 4, is almost always used because OS requires stack to be double-word aligned.

```nasm
; initial ESP = 0xb20000
mov eax, 0AAAAAAAAh
mov ebx, 0BBBBBBBBh
mov ecx, 0CCCCCCCCh
mov edx, 0DDDDDDDDh
push eax
; address 0xb1ffffc will contain the value 0xAAAAAAAA 
; and ESP will be 0xb1ffffc (=0xb200000-4)
push ebx 
; address 0xb1ffff8 will contain the value 0xBBBBBBBB
; and ESP will be 0xb1ffff8 (=0xb1ffffc-4)
pop esi
; ESI will contain the value at 0xb1ffffc (=0xb1ffff8+4) which is 0xBBBBBBBB
pop edi 
; EDI will contain the balue at 0xb200000 (=0xb1ffff8+4) which is 0xAAAAAAAA
```

![[Pasted image 20250115175058.png]]
_ESP_ can also be directly modified by other instructions, such as _ADD_ and _SUB_.
### Function Invocation
While high-level programming languages have the concept of functions that can be called and returned from, the processor doesn't provide such abstraction. At the lowest level, the processor operates only on concrete objects, such as registers or data coming from memory, and functions are implemented using the stack data structure.
Consider the following function ->
```C
int __cdecl addme(short a, short b) {
	return a + b;
}
```
the calling of this function would look like this; 
```C
int a = 3;
int b = 4; 
int c = addme(a, b)
```
In assembly these two code snippets would look like this; for the function 
#### RET Instruction
```nasm
push ebp
mov ebp, esp
; This two-instuction sequence in typically known as the 'function prologue'
; because it establishes a new function frame.
; it basically saves the original pointer to ebp on the stack
; and puts the stack pointer inside the ebp register
; ----------------------------------- Note ------------------------------------
; This proceadure is not necessery, but it makes it easier to track function
; variables as they are pushed and poped from the stack. 
; because everything, including the return address will be static distance
; from the ebp register
; optimizing functions to not do this is called 'frame pointer omission'
; -----------------------------------------------------------------------------

movsx eax, word ptr [ebp+8]
movsx ecx, word ptr [ebp+0Ch]
add eax, ecx
; This is the body of the function doing the add logic computation 

mov esp, ebp 
pop ebp
; This two-instruction sequence in tyipcally known as the 'function epilogue'
; because it restores the stack frame to what it was before the function call
; firstly is restores the stack pointer to what is was at the start
; which is the base pointer, then restores ebp 
; ebp was saved on the stack at the function prologue

retn
; this instruction pops the stack, and resumes execution at the address 
; located on the top of the stack
; basically 'pop eip'
```

The calling of this function would look like this ->
#### CALL Instruction
```nasm
push eax
push ecx 
; push the variables 'a' and 'b'

call addme
; this instruction firstly pushes the next instruction address to the stack
; then it changes the EIP pointer to the address of the add me function
; so it can continue the execution of the function there
; basically push [eip+4] (next instruction after the call) then mov eip, func

add esp, 8
; cleanup of the stack per __cdecl call convention, if the function 'addme' 
; had local variables, the code would need to grow the stack by substracting 
; ESP after the function prolouge, and then adding it back befor the retn 
; here we cleanup the stack of the main function 
```

#### <u>Calling Convention</u>
| .                      | CDECL                                                                                        | STDCALL                                                     | FASTCALL                                                                      |
| ---------------------- | -------------------------------------------------------------------------------------------- | ----------------------------------------------------------- | ----------------------------------------------------------------------------- |
| Parameters             | Pushed on top of the stack from right to left. Caller must clean up the stack after the call | Same as _CDECL_ except that the callee must clean the stack | First two parameters are passed in _ECX_ and  _EDX_ The rest are on the stack |
| Return Value           | Stored in _EAX_                                                                              | Stored in _EAX_                                             | Stored in _EAX_                                                               |
| Non-volatile registers | _EBP_, _ESP_, _EBX_, _ESI_, _EDI_                                                            | _EBP_, _ESP_, _EBX_, _ESI_, _EDI_                           | _EBP_, _ESP_, _EBX_, _ESI_, _EDI_                                             |
### Exercise
1.  EIP is a read-only register so '_mov eax, eip_' is not a valid instruction. 
	to read the value of EIP, we can use call some_function and then afterwards pop eax
	in the function which will put the value of eip in eax
	to intentionally change eip there is the jmp instruction or push with ret
```nsm
some_function:
	pop eax
call some_function
; this will read the eip register
```
1. 
```nasm
mov dword ptr [rsp], 0AABBCCDDh
ret
; This will put AABBCCDD in the stack in ret will take it and put it on EIP

mov eax, 0AABBCCDDh
jmp eax
```

1. If the function would change some variables on the stack, and push them, and will not properly restore the stack, the current return address won't be where it is expected, and the code will crush or return to some random address.
2. If the return type is bigger then what can fit in the return register (most of the time eax), then a pointer is implicitly give to the function that points on the structure or thing that is returned. 
### Control Flow
This section describes how the system implements stuff like if/else statements, switch/case, while/for which are implemented with the _CMP, TEST, JMP_ and _Jcc_
instruction where 'cc' stands for conditional code (equal, greater then, etc..)
These instructions work closely to the _EFLAGS_ registers, which has the following flags
- <u>ZF/Zero Flag</u> - set if the last arithmetic operations resulted in zero
- <u>SF/Sign Flag</u> - set to the most significant bit of the result
- <u>CF/Carry Flag</u> - set when the result requires carry (applies to unsigned numbers)
- <u>OF/Overflow Flag</u> - set if the result overflows (applies to signed numbers, i.e -1 = maxint)

| Conditional Code | English Description                                             | Machine Description |
| ---------------- | --------------------------------------------------------------- | ------------------- |
| B/NAE            | Below/Neither Above nor Equal. Used for unsigned operations     | CF=1                |
| NB/AE            | Not Below/Above or Equal. Used for unsigned operations          | CF=0                |
| E/Z              | Equal Zero                                                      | ZF=1                |
| NE/NZ            | Not Equal / Not Zero                                            | ZF=0                |
| L                | Less than/Neither Greater not Equal. Used for signed operations | (SF ^ OF)=1         |
| GE/NL            | Greater or Equal / Not Less than. Used for signed operations    | (SF ^ OF)=0         |
| G/NLE            | Greater / Not Less not Equal. Used for signed operations        | ((SF ^ OF) \| ZF)=0 |

Because assembly doesn't have a defined type system, one of the few ways to recognize signed / unsigned types is through these conditional codes.
#### CMP Instruction
Subtracts two registers one from another and changes the appropriate flags without saving the result. 
```nasm
cmp eax, ecx
jz
; jump if eax == ecx
; eax - ecx = 0 so jz (jump zero) will jump
```
#### TEST Instruction
Performs logical _AND_ between the two registers and changes the appropriate flags on the _EFLAGS_ register without saving the result
```nasm
test eax, eax
jnz
; jmp if eax != 0 
; eax & eax != 0 so jz (jumo not zero) will jump
```

### Condition Cases 
#### If-Else
If-Else constructs are simple to recognize because the involve compare/test followed by a _jmp_ of some type, for example:
```nasm
mov esi, [ebp+8]
; load some address to ESI

mov edx, [esi]
test edx, edx 
; load what is inside the address and check for zero, which means *esi = 0

jz short loc_4E31F9
; if zero, means *edx = 0. jump to location and return.
; else continue with the code bellow

mov ecx, offset _FsRtlFastMutexLookasideList 
; move offset to function, probably used in the call

call _ExFreeToNPagedLookasideList@8
; call the function _ExFreeToNPagedLookasideList

and dword ptr [esi], 0
; zero out what is inside address esi, *esi = 0;

lea eax, [esi+4]
push eax
call _FsRtlUninitializeBaseMcb@4
; then call the function _fsRtlUninitizlizeBaseMcb, probably a fast call
; with *(esi+4) as parameter

loc_4E31F9:
	pop esi
	pop ebp
	retn 4
_FsRtlUninitializeLargeMcb#4 endp
```
This code looks like this in C:
```C
if (*esi == 0) {
	return;
}
ExFreeToNpagedLookAsideList(...);
*esi = 0;
return;

// This code is the same as

if (*esi != 0) {
	 ExFreeToNpagedLookAsideList(...);
	 *esi = 0;
}
return; 
```
#### Switch-Case
A switch-case block is a sequence of if/else statements 
```C
switch (ch) {
	case 'c':
		handle_C();
		break;
	case 'h':
		handle_H();
		break;
	default:
		break;
}
domore();

// Same as 

if (ch == 'c') {
	handle_C();
} else if (ch == 'h') {
	handle_H();
}
domore();
```
Hence, the machine code translation will be a series if/else. The following simple example illustrates the idea:
```nasm
push ebp
mov ebp, esp
; start of a function

mov eax, [ebp+8]
; load variable from the stack

sub eax, 41h
jz short loc_caseA
; check eax == 'A' == 0x41

dec eax
jz short loc_caseB
; check if eax == 'B' == 0x42
; it it was bigger then 0x41, and removing one will make it 0,
; the number it is equal to was 0x41

dec eax
jz short loc_caseC
; check if eax == 'C' == 0x43

mov al, 5Ah
movzx eax, al
; move the 'Z' (0x5A) to the al register
; transfer al to eax, and because eax is 
; a larger register, pad it with zeros

pop ebp
ret

loc_caseC:
mov al, 43h
movzx eax, al
pop ebp  
retn
; move 'C' == 0x43 to the return value and return

loc_caseB:
mov al, 42h
movzx eax, al
pop ebp
ret
; move 'B' == 0x42 to the return value and return

loc_caseA:
mov al, 41h
movzx eax, al
pop ebp
ret
; move 'A' == 0x41 to the return value and return
```
Matching C Code
```C
char hex_to_ascii(int hex) {
	char result;
	switch (hex) {
		case 0x41:
			result = 'A';
			break;		

		case 0x42:
			result = 'B';
			break;
		
		case 0x43:
			result = 'C';
			break;

		default:
			result = 'Z';
			break;
	}
	return result;
}
```
Real-life switch-case statements cab be more complex, and compilers commonly build a _jump table_ to reduce the number of comparisons and conditional jumps. 
The jump table is an array addresses, each pointing to the handler for a specific case, This pattern can be observed in the following code: 
```nasm
cmp edi, 5
ja short loc_10001141
; if edi is greater the 5, jump 

jmp ds:off_100011A4 [edi*4]
; else jump to address in offset 100011A4 plus edi*4
; looks like a view into an array

loc_10001125:
mov esi, 40h
jmp short loc_10001145

loc_1000112C:
mov esi, 20h
jmp short loc_10001145

loc_100001133:
mov esi, 38h
jmp short loc_10001145

loc_1000113A:
mov esi, 30h
jmp short loc_10001145

loc 10001141:
mov esi, [esp+0Ch]
...

; This is the jump table, each index is a location
off_100011A4 
dd offset loc_10001125
dd offset loc_10001125
dd offset loc_1000113A
dd offset loc_1000112C
dd offset loc_10001133
dd offset loc_1000113A
```
Matching C code
```C
switch (edi) {

	case 0:
	case 1:
		// goto loc_1001125
		esi = 0x40;
		break;

	case 2:
	case 5:
		// goto loc_1000113A
		esi = 0x30;
		break;

	case 3:
		// goto loc_1000112C
		esi = 0x20;
		break;

	case 4:
		// goto loc_10001141
		esi = 0x38;
		break;

	default:
		esi = *(esp+0xC);
		break;

}
```
This method saves up on 10 additional instruction in this example to test each case and branch to the handler.
#### Loops 
At the machine level, loops are implemented using a combination of _Jcc_ and _JMP_ instructions. In other words, they are implemented using _if/else_ and _goto_ constructs. The best way to understand it is to rewrite a loop using only _if_, _else_ and _goto_. consider the following example: 
```C
// This is a regular loop in C
for (int i = 0; i<10; i++) {
	printf("%d\n", i);
}
printf("Done!\n");

// This is the same loop using if, else and goto
int i = 0;
loop_start: 
	if (i < 10) {
		printf("%d\n", i);
		i++;
		goto loop_start
	}
printf("Done");
```
When complied, these are looking the same on the machine-code level.
```nasm wrap
mov edi, ds:__imp_printf
; put the address of the printf function in edi

xor esi, esi
; zero out esi, 'int i = 0'
loc_401010:
	push esi
	push offset Format  
	; push the number i, and format string "%d\n" 
	; to the stack per __cdecl call convention
	call edi
	; call the printf function, and print the message
	inc esi
	; increment esi (i++)
	add esp, 8
	; clean the stack per cdecl call convention of printf
	cmp esi, 0Ah
	jl short loc_401010
	; jump to the start of the loop if i < 10
push offset aDone
; push the string "Done" to the stack
call edi
; call the printf function
add esp, 4
; clean the stack
```
#note
In this example we can know signed numbers were used because of the type of the jump instruction. <u>*Remember* - If "above/below" is used, it is unsigned; if "less than/greater than", it was done on signed integers</u>
Another interesting loop is this ->
```nasm
sub_1000AE3B proc near
push edi
;store the value on edi to restore later

push esi
; esi is probably the start address of the string

call ds:lstrlenA
; call the strlen function on a string 

mov edi, eax
; move the length to the edi register

xor ecx, ecx
xor edx, edx 
; zero out ecx & edx

test edi, edi
jle short loc_1000AE5B
; check the length of the string is less or equal to zero
; if so jump to the end of the function

loc_1000AE4D:
	mov al, [edx+esi]
	mov [ecx+esi], al
	add edx, 3
	inc ecx
	; al = str[edx]
	; str[ecx] = al
	; edx += 3
	; ecx++

	cmp edx edi
	jl short loc_1000AE4D
	; check if edx < len(str) if so, continue looping

loc_1000AE5B:
	mov byte ptr [ecx+esi], 0
	; put the null byte on the end of the new string 
	; new string length can be smaller so it needs 
	; to add the null byte just in case
	
	mov eax, esi
	pop edi
	; move the string address to the return register eax
	; and retrive the value of edi before the function
	
	ret
	; return to the main code
	
sub endp
```
Matching C code
```C
char* function(char* esi) {
	int length = lstrlenA(esi); // length of string
	int ecx = 0;
	int edx = 0;

	if (length == 0) { return NULL;}

	while (edx < length) {
		al = esi[edx];
		esi[ecx] = al;
		ecx++;
		edx+=3;
	}
	esi[ecx] = 0;
	return esi;

}
```
#### Loop Instruction
Some loop can use the loop instruction which executes a block of code up to _ECX_ times
```nasm
mov ecx, edx
loc_CFB8F:
	lodsd
	not eax 
	stodsd 
	loop loc_CFB8F
```
A matching C code would be
```C
while (ecx != 0) {
	eax = edi*;
	edi++;
	eax = ~eax;
	*esi = eax;
	esi++;
	ecx--;
}
```
# x64
_Learned via [[Practical Reverse Engineering.pdf#page=63|Practical Reverse Engineering]]_