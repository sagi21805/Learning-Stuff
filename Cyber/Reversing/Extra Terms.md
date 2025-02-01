# Interrupt Descriptor Table
The **Interrupt Descriptor Table** (**IDT**) is a binary data structure specific to the IA-32 and x86-64 architectures. The IDT entries are called <u>gates</u>. It can contain Interrupt Gates, Task Gates and Trap Gates. The location of the **IDT** is kept in the **IDTR** (**IDT** register). This is loaded using the **LIDT** assembly instruction, whose argument is a pointer to an **IDT Descriptor** structure
```C
typedef struct IDT {
	unsigned short limit;
	unsigned int / unsigned long long base; //x86_32 uses DWORD, x86_64 uses QWORD
}
```
This is practically an array of function pointers to handlers of interrupts, the handlers are called drivers and they most of the time signed by Microsoft. 
Each entry is 8 or 16 bytes long depending on the architecture. 

The IDT is at:
- 0x80ffffff in Windows
- 0xe8XXXXXX in Virtual PC
- 0xffXXXXXX in VMware

## Red Pill
Most of the time malware is analyzed on a virtual machine. and when the malware is analyzed it is ran by the user. In order to make the life harder on malware analysts, an advanced malware can change it's behavior if it can detect it is ran on a virtual machine. Red Pill is an anti-VM technique that executes the _SIDT_ instruction to grab the value of the IDTR register. The virtual machine monitor must relocate the guest’s IDTR to avoid conflict with the host’s IDTR. Since the virtual machine monitor is not notified when the virtual machine runs the _SIDT_ instruction, the IDTR for the virtual machine is returned. Red Pill tests for this discrepancy to detect the usage of VMware.
![[Pasted image 20250201191632.png]]
```nasm
mov al, [eax+5]
cmp al, 0FFh ; VMware signature in the IDT register
jnz short loc_401E19
```