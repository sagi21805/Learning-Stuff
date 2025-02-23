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
# Interrupt Service Routine (ISR)
Drivers for physical devices that generate interrupts have at least one _Interrupt Service Routine_
The ISR must do whatever is appropriate to handle the device interrupt or to dismiss it. Basically do the minimal effort to handle the interrupt or ignore it, Then, it should only do whatever is necessary to save the state and queue a [[#Deferred Procedure Call]] (DPC) to finish the I/O operation at a lower priority (IRQL) than that at which the ISR executed.
# Deferred Procedure Call (DPC)
Every driver that has an ISR most likely also have a DPC routine to complete the processing of the interrupt driven I/O operation, a typical routine does the following. 
- Finish handling I/O operation that ISR has begun processing
- Dequeues the next I/O Request Packet (IRP) so that the driver can begin processing it.
- Completes the current IRP, if possible
Sometimes the current IRP cannot be completed because several data transfers are required, or a recoverable error was detected. In these cases, another DPC routine typically reprograms the device for either another transfer or a retry of the last operation.
## How DPC is Used with ISR
For example, imagine a network packet entering the Network Card, the network card sends an interrupt to the CPU, and the ISR is triggered, which will acknowledge the packet that entered and will queue a DPC for the processing of the packet, if there are no more interrupt with higher IRQL the DPC is called and the packet is processed. 
# Asynchronous Procedure Calls
