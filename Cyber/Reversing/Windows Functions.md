ALL Win32 API functions follow the [[Practical Reverse Engineering#<u>Calling Convention</u>|__stdcall]] calling convention
# [CreateToolhelp32Snapshot](https://learn.microsoft.com/en-us/windows/win32/api/tlhelp32/nf-tlhelp32-createtoolhelp32snapshot)
This function takes a snapshot of the specified processes, as well as the heaps, modules, and threads used by these processes.
```c++
HANDLE CreateToolhelp32Snapshot(
  [in] DWORD dwFlags,
  [in] DWORD th32ProcessID
);
```
If the function succeeds, it returns an open handle to the specified snapshot.
# [Process32First](https://learn.microsoft.com/en-us/windows/win32/api/tlhelp32/nf-tlhelp32-process32first)
This function retrieves information about the first process in a system snapshot.
```C
BOOL Process32First(
  [in]      HANDLE           hSnapshot,
  [in, out] LPPROCESSENTRY32 lppe
);
```
if the function succeeds, it returns true, and the process information is saved on the process entry pointer. structure
# [Process32Next](https://learn.microsoft.com/en-us/windows/win32/api/tlhelp32/nf-tlhelp32-process32next)
This function retrieves information about the next process in a system snapshot
```
BOOL Process32Next(
  [in]  HANDLE           hSnapshot,
  [out] LPPROCESSENTRY32 lppe
);
```
# [\_STRICMP](https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/stricmp-wcsicmp-mbsicmp-stricmp-l-wcsicmp-l-mbsicmp-l?view=msvc-170)
This function is compares two strings without case sensitivity.
```c
int _stricmp(
	const char *string1
	const char *string2
);
```
Both of the strings are null terminated

