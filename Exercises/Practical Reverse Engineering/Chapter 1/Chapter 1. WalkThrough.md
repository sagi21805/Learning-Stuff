This is a summarizing _DLLMain_ routine with less the 100 assembly lines. This function applies almost every concept covered in the chapter above and It covers an important part The function ->

```assembly
BOOL __stdcall DllMain(HInstance hinstDLL, DWORD fdwReason, LPVOID lpvReserved) 
_DLLMain@12 proc near
push ebp
mov ebp, esp
sub esp, 130h
; function start and allocate 0x130 bytes on the stack.

push edi
; save edi

sidt fword ptr [ebp-8] 
; writes the interrupt descriptor register table into a memory location 
; information on that table on the link bellow

mov eax, [ebp-6]
; read from memory location ebp-6 into eax
; when checking the structure of the IDT register and in little endien 
; eax contains the base address of the IDT

cmp eax, 8003F400h
jbe short loc_10001C88
; compare base with 0x8003F400 if eax is bellow or equal jump to loc_10001C88.

cmp eax, 80047400h
jnb short loc_10001C88 
; comapre base with 0x80047400 if eax is above or equal jump to loc_10001C88

; 8003F400h and 80047400h are not just any numbers
; they are the base address of the IDT on windows XP
; basically if the idt base address is valid, jump else return

xor eax, eax
; zero out eax for return status

pop edi
mov esp, ebp
pop ebp
retn 0Ch
; return edi for what it was at the start of the function and restore the stack

loc_10001C88:
; jump here if [ebp-6] which is eax is between 0x8003F400 and 0x80047400
; which means the base of the idt is valid, and running on windows XP 

xor eax, eax 
; zero out eax

mov ecx, 49h
; move 73 (0x49) to ecx, probably a conter

lea edi, [ebp-12Ch]
; load address of ebp-300 into edi

mov dword ptr [ebp-130h], 0
; load zero into ebp-304 

push eax
push 2
; push to the stack eax (0) and 2 
; will be used for the function call bellow

rep stosd
; write the value of eax (0) into edi ecx (73) times
; because it writes double words, it zeros out [ebp-300] - [ebp-8]

call CreateToolhelp32Snapshot
; call the CreateToolhelp32Snapshot function 
; first parameter 2, second parameter eax (0)
; 2 is flag TH32CS_SNAPPROCESS which is accroding to docs ->
; Includes all processes in the system in the snapshot. 
; To enumerate the processes, see Process32First
; If the function succeeds, it returns an open handle to the specified snapshot.
; Now eax is an open handle to the snapshot or INVALID_HANDLE_VALUE which is -1

mov edi, eax
; save the handle on edi

cmp edi, 0FFFFFFFFh 
jnz short loc_10001CB9
; if the function above returned a valid value and not INVALID_HANDLE_VALUE
; jump to location loc_10001CB9 
; which means a snapshot of the whole system was taken 

xor eax, eax
pop edi
mov esp, ebp
pop ebp
; zero out eax, and restore edi and the stack frame

retn 0Ch
; non zero return value which can help debug errors

loc_10001CB9:
; if jumped here, a snapshot of all the processes was taken
; eax is the handle for that snapshot
; the address of the handle is also on edi

lea eax, [ebp-130h]
; load the address of the start of the struct of tagPROCESSENTRY32

push esi
; not sure why this is pushed, the function takes two arguments
; maybe to save esi

push eax
push edi
; push the second and first argument of the function to the stack 

mov dword ptr [ebp-130h], 128h
; set the size of the struct to 296 bytes

call Process32First
; call the function Process32First 
; which Retrieves information about the first process in a system snapshot

test eax, eax
jz short loc_10001D24 
; check if return value is 0 (False) if so jump to loc_10001D24

mov esi, ds:_stricmp
; move to esi the address of stricp
; this function compares strings without case sensivity
; which means "TEST" == "test"

lea ecx, [ebp-10Ch]
; we know that ebp-0x130 points to tagPROCESSENTRY32
; which means this is a field in this structure
; when taking the offset we can see it is the szExeFile[MAX_PATH] field
; which is a char* (string) field of the name of the executealbe

push 10007C50h
push ecx
call esi ; _stricmp
add esp, 8
; the arguments for the _stricmp function are two strings
; 0x10007C50 is the address of the first one (known to be 'explorer.exe')
; ecx which is the name of the exe of the process is string two
; then the stack is restored per to cdecl calling convention

test eax, eax
jz short loc_10001D16 
; if the strings match, jump to loc_10001D16

loc_10001CF0:
; if strings don't match continue execution here

lea edx, [ebp-130h]
; load the address of the start of the struct of tagPROCESSENTRY32

push edx
push edi
call Process32Next
; push the pointer to the process entry and the snapshot to the stack
; and use them as arguments to Process32Next 

test eax, eax
jz short loc_10001D24
; check if a process has been copied to the buffer
; if so continue execution, if it was not copied jump to loc_10001D24

lea eax, [ebp-10Ch]
push 10007C50h
push eax
call esi ; _stricmp
add esp, 8
test eax, eax
jnz short loc_10001CF0
; similar code block to what we have seen before
; check if the executable name is 'explorer.exe' 
; if the strings don't match do the whole block agains (while)

loc_10001D16:
mov eax, [ebp-118h]
; move the field thParentProcessID to eax
mov ecx, [ebp-128h]
; move the field thProcessID to ecx
jmp short loc_10001D2A

loc_10001D24:
mov eax, [ebp+0Ch]
mov ecx, [ebp+0Ch]
; move the same value to eax and ecx 

loc_10001D2A:
cmp eax, ecx
pop esi
jnz short loc_10001D38
; this cmp and jump checks if we came from loc_10001D16 or loc_10001D24
; if from loc_10001D16 fields don't match, so
xor eax, eax
pop edi
mov esp, ebp
pop ebp
retn 0Ch
; if got here from 10001D24 exist the funtion and restore the stack frame

loc_10001D38:
; eax and ecx contains the thParnetProcessID and thProcessID
mov eax, [ebp+0Ch]
; this is the fdwReason argument in the function
; each argument is 32bit wide so the stack of the function is like this ->
; ebp+0 -> saved ebp
; ebp+4 -> saved return address
; ebp+8 -> first argument (hinstDLL)
; ebp+C -> second argument (fdwReason)

dec eax
jnz short loc_10001D53
; The DLLMain function is called when a DLL is un/loaded into a process memory
; the dec eax, and jump checks if it is a fwdReason of type DLL_PROCESS_ATTACH
; This parameter checks if it is loaded.
; if it is not loaded, the funciton jumps to loc_10001D53

push 0
push 0
push 0
push 100032D0h
push 0
push 0
call ds:CreateThread
; if the DLL is loaded and attached to the process it creates a thread
; with parameters 
; lpThreadID            = NULL
; dwCreationFlags       = NULL
; lpParameter           = NULL
; lpStartAddress        = 0x100032D0
; dwStackSize           = NULL
; LPSECURITY_ATTRIBUTES = NULL
; This creates a thread on the process that starts the function*- at 0x100032D0 

loc_10001D53:
mov eax, 1
pop edi
mov esp, ebp
pop ebp
retn 0Ch
; this just exists the function and restores the stack frame and edi

_DllMain@12 endp
```

### Re-Decompile the Walk-Through

This can be converted to the following C code;

```c
typedef struct IDTR {
	short limit;
	int base;
}

typedef struct tagPROCESSENTRY32 {
  DWORD     dwSize;
  DWORD     cntUsage;
  DWORD     th32ProcessID;
  ULONG_PTR th32DefaultHeapID;
  DWORD     th32ModuleID;
  DWORD     cntThreads;
  DWORD     th32ParentProcessID;
  LONG      pcPriClassBase;
  DWORD     dwFlags;
  CHAR      szExeFile[MAX_PATH]; 
} PROCESSENTRY32;

Bool __stdcall DllMain(HInstance hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
	IDTR idtr;
	__sidt(&idtr); // write the data of the sidt instruction to the struct
	if (idtr.base > 0x80047400 || idtr.base <= 0x8003F400) {
		HANDLE h = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		PROCESSENTRTY32 entry;
		memset(&entry, 0, sizeof(entry));
		if (h != INVALID_HANDLE_VALUE){
			entry.dwSize = sizeof(entry); //i.e 0x128 bytes
			bool copied = Process32First(&h, &entry);
			if (copied) {
				if (fwdReson != DLL_PROCESS_ATTACH) { return 12; }
				int status = _stricmp(entry.szExeFile, "explorer.exe");
				while (status != 0 && copied) {
					copied = Process32Next(&h, &entry);
					status = _stricmp(entry.szExeFile, "explorer.exe");
					if (!copied) { return;}
				}
				CreateThread(NULL, NULL, 0x100032D0, NULL, NULL, NULL);
				// 0x100032D0 Probably address to malicious function
			}
		}
		return 12;
	}
	// if address is not valid return
	return 12;
}
```

This code basically:

1. Checks if the IDT of the system is valid for x86 windows xp
2. Check if the user logged in, i.e explorer.exe is running
3. Run malicious program if the two terms above are true