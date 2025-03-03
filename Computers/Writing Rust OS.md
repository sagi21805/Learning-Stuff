_This is done on the repository [RustOS](https://github.com/sagi21805/RustOS) with help from [Writing OS in Rust](https://os.phil-opp.com/) and other sources!_
# Bare Metal Binary
## Assembly
In assembly it is much simpler to create a standalone binary, because all the instructions we write are OS dependent.
Simply compile the assembly with `nasm -f bin some_asm_file.asm -o binary.bin`
For example, simple Hello World on boot will be ->
```nasm
bits 16 ; tell NASM this is 16 bit code
org 0x7c00 ; tell NASM to start outputting stuff at offset 0x7c00
boot:
    mov si,hello ; point si register to hello label memory location
    mov ah,0x0e ; 0x0e means 'Write Character in TTY mode'
.loop:
    lodsb
    or al,al ; is al == 0 ?
    jz halt  ; if (al == 0) jump to halt label
    int 0x10 ; runs BIOS interrupt 0x10 - Video Services
    jmp .loop
halt:
    cli ; clear interrupt flag
    hlt ; halt execution
hello: db "Hello world!",0

times 510 - ($-$$) db 0 ; pad remaining 510 bytes with zeroes
dw 0xaa55 ; magic bootloader magic - marks this 512 byte sector bootable!
```
## Rust
Most of the time when we compile a binary, this is an operating system specific binary. It's starting with the OS's headers, and most of the time implicitly links the standard library. 
Because we have no OS, we can't do that, so we must disable that.
```rust
#![no_std]
fn main() {

}
```
When doing so, rust will require us to implement the <u>panic</u> function, which is the function that is called when a panic raises, this function returns the [never](https://doc.rust-lang.org/nightly/std/primitive.never.html) which indicates it doesn't return anything. (most of the time, rust functions that don't return anything will return () )
```rust
use core::panic::PanicInfo;

/// This function is called on panic.
#[panic_handler]
fn panic(_info: &PanicInfo) -> ! {
    loop {}
}
```
The panic info is just a struct, that has information about the panic that has been raised.
```rust
pub struct PanicInfo<'a> {
	message: fmt::Arguments<'a>, // Message
	location: &'a Location<'a>   // Line of occurance in code
	can_unwind: bool,            // unwind to free allocated vars
	force_no_backtrace: bool,   
}
```
### Disable Unwinding
Unwinding happens when memory is needed to be freed, but the binary panicked so the memory cannot be freed, unwinding calls the drop function of any structure that implements it, so all memory will be freed. Since we have no OS, no memory is considered 'leaked' 
To disable add the following lines in 'cargo.toml'
```toml
[profile.dev]
panic = "abort"

[profile.release]
panic = "abort"
```
### The _\_start_ Attribute
One might think that the main function is called when you start a program. However, most languages have a _runtime system_ which is responsible for language specific things such as garbage collection (e.g. Jave) software threads (e.g. goroutines in Go). This runtime needs to be called before main, so it can initialize itself.
In a typical rust binary, that links the standard library, execution starts by running the C runtime library called `crt0` (C Runtime Zero) which sets up  the environment for a C application. This includes creating a stack and placing arguments in the right registers. This then  invokes the entry point of the rust runtime, which is marked by the `start` language item. Rust has a very minimal runtime, that sets stack overflow guards, and and prints backtrace, and finally calling the main function.
```rust
pub extern "C" fn _start() -> ! {
// this function is the entry point, since the linker looks for a function
// named `_start` by default but it can, and will be change for the bootloader
}
```
### Libraries That Can Be Used
Even if we don't have an operating system, some libraries can still be used for some independent useful rust funcionality.
#### Core
This library adds the basic rust functionality, like slices, cstr, &str, Option, result, etc.
#### Alloc
This includes certain memory functions like Box, Arc, Linked List etc.
To use alloc, a global heap allocator to a programs heap must be configured, only one allocator can be in a program, so make sure any crate the program depends on don't have it's own allocator. 
### How to Include Them
This can be only done in the nightly version using. To use rust nightly, include a `toochain.toml` file in the project directory that includes ->
```toml
[toolchain]
channel = "nightly"
components = ["rustfmt", "clippy", "rust-src", "llvm-tools-preview"]
targets = ["x86_64-unknown-none"]
```
Then build with
```bash
cargo build --release --target i386-code16-boot-sector.json -Zbuild-std=core
```
The '-Z' flag includes a nightly only features, the <u>build-std=core</u> instructs cargo to build the core library.
## Compile For Raw 16-bit Binary
For this we need to add a custom target to rust that is 16-bit, luckily, llvm supports the i386 CPU which is 16-bit, so rust can compile to it!
The target is a JSON containing some configuration ->
```JSON
{
"arch": "x86", // x86 cpu architecture
"cpu": "i386", // The i386 cpu which has support for 16-bit instructions
"data-layout": "e-m:e-p:32:32-p270:32:32-p271:32:32-p272:64:64-i128:128-f64:32:64-f80:32-n8:16:32-S128",
// Data layout which is splited by '-'
// e -> is for little endien (E for big)
// m:3 -> for Elf style name mangling
// p:32:32 -> 32-bit pointer with 32-bit address space 
// p270:32:32 -> special pointer type ID-270 with 32-bit size and allignment
// p271:32:32 -> special pointer type ID-271 with 32-bit size and allignment
// p271:64:64 -> special pointer type ID-272 with 64-bit size and allignment
// i128:128 -> 128-bit integers are 128-bit alligned
// f64:32:64 -> 64-bit floats are 32-bit alligned, can be 64-bit alligned
// n:8:16:32 -> native integers are 8-bit, 16-bit, 32-bit
// S128 -> stack is 128-bit allgined
"dynamic-linking": false, // dynamic linkning is disabled
"executables": true, // suppory executables
"linker-flavor": "ld.lld", // use llvm linker (instead of gnu for example)
"linker": "rust-lld", // same as above
"llvm-target": "i386-unknown-none-code16", // tell llvm this is the 16-bit cpu
"max-atomic-width": 64, // the widest atomic operation is 64-bit 
"position-independent-executables": false, 
// position independed executables are disabled 
// the position of this executable matters 
// because it is loaded in a specific memory (0x7c00)
"disable-redzone": true, 
// disable the redzone optimization, which saves in advance memory
// on a functions stack without moving the stack pointer which saves
// an instruction, this is a convention, which means that the guest OS
// won't overrite this otherwise 'volitile' memory
"target-c-int-width": "32", // normal c int's are 32-bit
"target-pointer-width": "32", // regular pointers are 32-bit 
"target-endian": "little", // endieness, little or big
"panic-strategy": "abort", 
// panic startegy, also set on cargo.toml
// this aborts execution instead of unwinding 
"os": "none", // no target os 
"vendor": "unknown", // same as above
"relocation-model": "static" // the code is statically linked
}
```
The data layout (which is the most complicated one) is copied from the [i386_apple_ios target](https://github.com/rust-lang/rust/blob/master/compiler/rustc_target/src/spec/targets/i386_apple_ios.rs) that is provided by rust
# Writing a Bootloader
_This is done with the help of [Writing a Bootloader](http://3zanders.co.uk/2017/10/13/writing-a-bootloader)_ , [OSDev](https://wiki.osdev.org/Expanded_Main_Page), and [this](https://github.com/rust-osdev/bootloader) repo 
We are going to write a floppy disk bootloader because it doesn't require us to mess out with filesystems, which helps to make things as simple as possible.
## BIOS
_OSDev was a great source of reading on [bios](https://wiki.osdev.org/BIOS)_

When pressing the power button, a certain software that is called the 'Basic I/O System' this software is stored in a read only memory on the motherboard and it has a lot of capabilities.
This software loads the first sector (only 512 bytes!) from a media device (i.e floppy disk, cdrom) if the last two bytes are equal to `0xAA55` which is the 'bootable signature'. Then bios jumps to location `0x7C00` which effectively transfers control to custom software. Most of the time this is when the bootloader is called.

At the point where the bios loaded the first sector, the CPU runs in 16-bit mode, so only 16-bit registers are available. Also since bios loads only the first sector, this means the boot loader code must be simple and tiny
A simple 'Hello World' would look like this ->
```nasm
bits 16    ; tell NASM this is 16 bit code
org 0x7c00 ; tell NASM to start base address at offset 0x7c00
		   ; this means segment addresses are offsted by 0x7c00
		   ; for example the mv si, hello
		   ; hello segment is stored at address 0x10
		   ; but in the actual code is offsted by 0x7c00 to 0x7c10
boot:
    mov si,hello ; point si register to hello label memory location
    mov ah,0x0e ; 0x0e means 'Write Character in TTY mode'
.loop:
    lodsb
    or al,al ; is al == 0 ?
    jz halt  ; if (al == 0) jump to halt label
    int 0x10 ; runs BIOS interrupt 0x10 - Video Services
    jmp .loop
halt:
    cli ; clear interrupt flag
    hlt ; halt execution
hello: db "Hello world!",0

times 510 - ($-$$) db 0 ; pad remaining 510 bytes with zeroes
dw 0xaa55 ; magic bootloader magic - marks this 512 byte sector bootable!
```
### Bios Interrupt Calls
Bios implementations provide interrupts that can be invoked by operating systems and application programs to use the facilities to the firmware.
These interrupts can control the screen,  disk, keyboard, etc and basically perform I/O control.
A key element of the purpose of BIOS calls is abstraction - the BIOS can perform generally defined functions, and the specific details of how those functions are executed on a system hardware are encapsulated in the BIOS and are hidden from the program. 
For example if a program wants to read from disk, it doesn't need to know if it is SATA or SCSI or other protocols, the program just needs to specify the disk number, and the sectors it wants to read. This makes it easier to make programs that interact with the hardware, and simplifies things for the programmer. 
This is how we will interact with the hardware, write to disk, and print to screen!
#### Common Functions
##### Video
- INT 0x10, AH = 1 -- set up the cursor
- INT 0x10, AH = 3 -- cursor position
- INT 0x10, AH = 0xE -- display char
- INT 0x10, AH = 0xF -- get video page and mode
- INT 0x10, AH = 0x11 -- set 8x8 font
- INT 0x10, AH = 0x12 -- detect EGA/VGA
- INT 0x10, AH = 0x13 -- display string
- INT 0x10, AH = 0x1200 -- Alternate print screen
- INT 0x10, AH = 0x1201 -- turn off cursor emulation
- INT 0x10, AX = 0x4F00 -- video memory size
- INT 0x10, AX = 0x4F01 -- VESA get mode information call
- INT 0x10, AX = 0x4F02 -- select VESA video modes
- INT 0x10, AX = 0x4F0A -- VESA 2.0 protected mode interface
##### Disk
- INT 0x13, AH = 0 -- reset floppy/hard disk
- INT 0x13, AH = 2 -- read floppy/hard disk in CHS mode
- INT 0x13, AH = 3 -- write floppy/hard disk in CHS mode
- INT 0x13, AH = 0x15 -- detect second disk
- INT 0x13, AH = 0x41 -- test existence of INT 13 extensions
- INT 0x13, AH = 0x42 -- read hard disk in LBA mode
- INT 0x13, AH = 0x43 -- write hard disk in LBA mode
##### Memory
- INT 0x15, EAX = 0xE820 -- get complete memory map
- INT 0x15, AX = 0xE801 -- get contiguous memory size
- INT 0x15, AX = 0xE881 -- get contiguous memory size
- INT 0x15, AH = 0x88 -- get contiguous memory size
- INT 0x15, AH = 0xC0 -- Detect MCA bus
- INT 0x15, AX = 0x0530 -- Detect APM BIOS
- INT 0x15, AH = 0x5300 -- APM detect
- INT 0x15, AX = 0x5303 -- APM connect using 32 bit
- INT 0x15, AX = 0x5304 -- APM disconnect
##### Keyboard
- INT 0x16, AH = 0 -- read keyboard scancode (blocking)
- INT 0x16, AH = 1 -- read keyboard scancode (non-blocking)
- INT 0x16, AH = 3 -- keyboard repeat rate
## Enabling A20 Line
_This was taken from [A20_Line](https://wiki.osdev.org/A20_Line)_
The A20-Line is one of the electrical lines that makes a system bus of an x86 based computer. The A20-Line in particular is used to transmit the 21-bit on the address bus.
A micro-processor typically has a couple of address lines equal to $\log_2(x)$ of the number of addresses it can reference, so a processor that references 4 GB of addresses which is $2^{32}$ bytes will have $log_2(2^{32})$  address lines which is 32 named A0-A31.
Historically, this is a backwards compatibility thing, for the development of the bootloader, we enable the A20-Line because it gives us access to all the memory addresses, instead of just one Megabyte.
It is better to test if the A20-Line was enabled by the bios before enabling it by ourselves
I won't go through the assembly way of doing it, because today hardware has a fast way of enabling this line which is much faster and simpler.
### Enabling
This is a manufacturer way of enabling it, and may not work on all mother boards.
```nasm
in al, 0x92  
test al, 2    
jnz after
or al, 2
and al, 0xFE
out 0x92, al
after:
```
There is also a way with BIOS int 0x15 to do so, but will be covered here as the above method works on qemo :)
## Disk Access
	### Disk Packet



