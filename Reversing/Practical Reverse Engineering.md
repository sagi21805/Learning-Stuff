# x86 and x64
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
|          |                                         |
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
### MOV Instruction
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
; And gates address at ecx+0x1C (Offset to DpcData) with 0 (Effictive set DcpData to 0)

mov [ecx+0Ch], eax
; Move value at EAX to address ECX+0x0C (offset to DeferredRoutine)

mov dword ptr [ecx], 113h
; Move value 0x113 to the ecx address, and enforce it to be a 32-bit address (Because 0x113 is a 16-bit value the address will try to only change the first 16 bits but we want to change a 32bit address so we add dword ptr)  

mov [ecx+10h], eax
; Change the value at ECX+0x10 (Which is DefferedContext) to the value at EAX
```
On line 4, we write the double-word value 0x113 to the base of the structure. This is writing double-word value into a byte, which will overwrite the values after it. 
The code at line 4 could also be written less efficiently like this:
```asm
mov byte ptr [ecx], 13h
mov byte ptr [ecx+1], 1
mov word ptr [ecx+2], 0
```
The compiler decided to fold three instructions into one because it knew the constants ahead of time, and wanted to save space. 
### <u>Array Access</u>
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

```Pseudo code
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
### <u>LEA Instruction</u>
Load Effective address instruction loads the address of something. 
for example, if eax is an address in memory, and we want to get address eax + ebx * 3
```
mov ecx, eax + ebx * 3   
; This is not valid 

mov ecx, [eax + ebx * 3] 
; This will return what is inside that address

lea ecx, [eax + ebx * 3] 
; This will load the effective address, like wanted
```
### <u>REP Instruction</u>
Repeats an instruction the number in _ecx_ times 
for example 
```nasm
mov ecx, 10
rep scasb 
```
Will repeat the scasb command 10 times
### <u>REPNE Instruction</u>
This instruction repeats while ZF and or ecx is not zero, this instruction also decrements ecx first, while doing so and can be see like this.
```C
while (1) {
	ecx--;
	if (ZF == 1) || (ecx == 0) {break}
}
```
### <u>MOVS Instruction</u>
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
SCAS (Scan String) and STOS (Store String)
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

```nasm
 _STOS_ is the same as _SCAS_ except it write the value _AL_/_AX_/_EAX_ to _EDI_. It is commonly used to initialize a buffer to a constant value (such as _memset()_)
```
xor eax, eax
; set eax to 0

push 9 
pop ecx
; set ecx to 9

mov edi, esi
; set edi to the same address as esi

rep stosd
; write ecx * double-word bytes which in this case 36
```
This is equivalent to ->
```C
memset(edi, 0, 36)
```
### LODS Instruction
Another instruction from the same family. It reads a 1-, 2-, or 4- byte value from _ESI_ and stores it in _AL_/_AX_/_EAX_
## <u>Exercise</u>
Explain what is the type of _[EBP + 8]_ and _[EBP+C]_ in line 1 and 8, respectively
next, explain what this snippet does.
1) Type of _[ebp+8]_ == char*
2) Type of [ebp+0Ch] == char
```
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
## <u>System Mechanism</u>
## <u>Walk Through</u>
## <u>Exercises</u>

# x64
## <u>Register Set and Data Types</u>
## <u>Data Movement</u>
## <u>Canonical Address</u>
## <u>Function Invocation</u>
## <u>Exercises</u>