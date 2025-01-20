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