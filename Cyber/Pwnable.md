# Toddler's Bottle
## File Descriptor (fd challenge)
The given code was reading from a file descriptor, which is a process unique identifier for a file, which it can read or write from. Every process in Linux has 3 default file descriptors: 
0. stdin -> standard input
1. stdout -> standard output
2. strerr -> standard error
The code read from a given file descriptor minus 0x1234 offset and needed the string "LETMEWIN\n"
<u>Solve</u>
```bash
echo 'LETMEWIN' | ./fd 4660 
```
This puts the string "LETMEWIN\n" in the standard input because of the pipe, then the read is from file descriptor 0 which is the input because 4660 - 0x1234 is 0 (4660 decimal = 1234 hex)

## Find Hash Collision (collision challenge)
The challenge give a simple hash function that takes 4 bytes of the string each time, treats them as an int and sums them, then compares it to the hash of a password inside it. to get the right password just 
The given hash is 0x21DD09EC
To recreate something that passes the hash can just print all the hex numbers on after the other and then all zeros (Order is little endian)
```
./col $(python -c "print '\xec\x09\xDD\x21' + '\x00'*16")
```
This will not work because _strlen_ which is used to check the length of the string, will say the length is 4 because it stops when there is the 0 character, so every character must be at least one hex.
this means every int after our number will be 0x01010101 and there are 4 ints to the total number to subtract is 0x04040404
```
0x21DD09EC - 0x04040404 = 0x1DD905E8
```
so the bytes are \xe8\x05\xd9\x1d to print.
<u>Solve</u>
Python3 by default treats strings as UTF-8 and not byes, if it is printed with the 'b' prefix to the string, the output is a textual representation and to really write it we need the sys module 
```bash
./col $(python3 -c "import sys; sys.stdout.buffer.write(b'\xe8\x05\xd9\x1d' + b'\x01'*16)")
```
In python 2.7 when we print it prints the raw bytes to the stdout, so it just works.
```bash
./col $(python -c "print '\xe8\x05\xd9\x1d' + '\x01'*16")
```
## Buffer Over Flow (bof challenge)
The challenge was a simple function that uses the gets function for input, and it is needed to be overflowed to overwrite the key and get a shell to print the flag.
After reversing the function and analyzing it, this is what we get
![[Pasted image 20250120190239.png]]
We want to overflow the buffer until the key offset, and then put the right key, and for that we need to understand the stack of this function
_Assuming the base address EBP is zero_

| Address     | Value                          |
| ----------- | ------------------------------ |
| (-44)-(-12) | Input buffer to overflow       |
| (-12)-(-8)  | buffer overflow protrction     |
| (-8)-0      | # empy? (not relevant)         |
| 0-4         | saved EBP from main            |
| 4-8         | saved EIP from the _call func_ |
| 8-12        | KEY                            |
After tracing the stack, we can infer how much we need to overflow the buffer, for example for this buffer, we need to overflow -44 which is the start of the buffer, all the way to +8 where the key is located, although this will overwrite the return pointer, which typical buffer overflow will want to target, it will continue the jump to a shell so we don't mind that. 
The wanted key is the number 0xCAFEBABE, so we need to input it backwards because of little endieness
#note 
Another interesting thing to notice is the _mov eax, large gs:14h_ instruction which reads from local thread storage into the eax register. this is a buffer overflow check, that is between our buffer and other variables, and it is check in the end of the function to defend the _EIP_ pointer, but it does not affect us, because we want to run a shell.

The command to overflow it in the correct is 
```bash
(python3 -c 'import sys; sys.stdout.buffer.write(b"a"* 52 + b"\xbe\xba\xfe\xca\n")'; cat) | nc pwnable.kr 9000
```
This command write 52 time the letter 'a' which overflows the buffer until the key (52 = 44 + 8), then we insert the bytes of the key in reverse, and we use the cat command so the session between us and the server will not close. then we forward it to the server with |, and we get the shell, in which we can write _cat flag_ to get the flag. 
