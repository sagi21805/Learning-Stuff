# x86 and x64
- This is a little-endian architecture based on the Intel 8086 processor.
- This processor can operate in two mode _Real_ and _Protected_
<u>Real</u> - The processor state when it is first powered and support 16-bit instruction set
<u>Protected</u> - The processor state supporting virtual memory, paging, and other features.
This architecture supports multiple rings (0-3) for privileged, and most commonly uses ring 3 for the user and ring 0 to the kernel, this is controlled with the _cs_ register and called _CPL_ (current privilege level)
## <u>Registers and Data Types</u>
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
<u>Common Data Types</u>
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
```armasm
LDR  R3, [R3]       
; Read the value at address R3

ADDS R2,  R3, #1    
; Add one to it

STR  R2, [R3]       
; Write updated value back to address R3
```
<u>x86 Assembly</u>
```asm
inc dword ptr [eax] 
; Directly increment value at address EAX
```

Another important characteristic is that x86 uses variable-length instructions size: The instruction length can range from 1 to 15 bytes. On ARM, instructions are either 2 or 4 bytes in length.
### <u>Data Movement</u>
Instructions operate on values that come from registers or main memory, The most common instruction for moving data is _MOV_. The simplest usage is to move a register or immediate to a register. For example:
```asm
mov esi, 0F003Fh 
; Set ESI = 0xF003 (h for hex)

mov esi, ecx     
; Set ESI = ECX 
```
The next common usage is to move data to/from memory. Similar to other assembly conventions, x86 uses square brackets ([]) to indicate memory access. (The only exception is the _LEA_ instruction, which uses [] but does not actually reference memory.) 
<u>Simplest use case:</u>
```asm
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
```
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
<u>Arrays</u>
Array access is most of the time accessed in the general way
array[base + index * size_of_element]
```asm
mov esi, _KdLogBuffer[esi*4] 
; _KdLogBuffer is the base address 
; ESI in this case is the index, and the elements size is 4
```
This can be observed in code looping over an array
```asm
loop_start:
	mov eax, [edi+4]
	; mov address from offset edi+4 to eax
	; edi is probably an address in memory
	; This can be infered because in the next line it 
	; looks like indexing an array of ints
	
	mov eax [eax+ebx*4]
	; load the ebx index of the array into eax
	
	test eax, eax
	; tests if eax == eax, which is true, so it sets the ZF  
	; flag into 0
	
	jz short loc_7F627F
	; jumps into a location in memory if the ZF flag is 0 
	; which it is because of last line
	
loc_7F627F:
	inc ebx
	; Add one to the ebx register
	; compare ebx, with what is inside the address at edi
	; 

	cmp ebx, [edi]
	jl short loop_start

```
## <u>Exercise</u>
## <u>System Mechanism</u>
## <u>Walk Through</u>
## <u>Exercises</u>

# x64
## <u>Register Set and Data Types</u>
## <u>Data Movement</u>
## <u>Canonical Address</u>
## <u>Function Invocation</u>
## <u>Exercises</u>