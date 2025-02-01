ALL Win32 API functions follow the [[Practical Reverse Engineering#<u>Calling Convention</u>|__stdcall]] calling convention
# [CreateToolhelp32Snapshot](https://learn.microsoft.com/en-us/windows/win32/api/tlhelp32/nf-tlhelp32-createtoolhelp32snapshot)
This function is from the <tlhelp32.h> 
Takes a snapshot of the specified processes, as well as the heaps, modules, and threads used by these processes.
```c++
HANDLE CreateToolhelp32Snapshot(
  [in] DWORD dwFlags,
  [in] DWORD th32ProcessID
);
```
If the function succeeds, it returns an open handle to the specified snapshot.