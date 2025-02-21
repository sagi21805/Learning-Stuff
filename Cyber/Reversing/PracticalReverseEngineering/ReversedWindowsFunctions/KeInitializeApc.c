typedef unsigned char UCHAR;
typedef unsigned long ULONG;
typedef void* PVOID;
typedef struct LIST_ENTRY {} LIST_ENTRY;
typedef char CCHAR;
typedef enum bool {true, fales} BOOLEAN;
typedef enum KPROCESSOR_MODE {kernel, user} KPROCESSOR_MODE;

typedef enum _KAPC_ENVIRONMENT {
    OriginalApcEnvironment,
    AttachedApcEnvironment,
    CurrentApcEnvironment,
    InsertApcEnvironment
} KAPC_ENVIRONMENT, *PKAPC_ENVIRONMENT;

typedef struct _KTHREAD {
    // info in ida
    KAPC_ENVIRONMENT ApcStateIndex;
} *KTHREAD;

// size=0x58
typedef struct _KAPC { 
    // 0x00
    UCHAR Type;
    // 0x01
    UCHAR SpareByte0;       
    // 0x02
    UCHAR Size;              
    // 0x03
    UCHAR SpareByte1;         
    // 0x04
    ULONG SpareLong0;           
    // 0x08
    struct _KTHREAD *Thread; 
    // 0x10
    // Entrey to a list, probably a linked list
    LIST_ENTRY ApcListEntry;  
    // 0x20
    PVOID Reserved[3];      
    // 0x38
    PVOID NormalContext;
    // 0x40
    PVOID SystemArgument1;
    // 0x48
    PVOID SystemArgument2;
    // 0x50
    CCHAR ApcStateIndex;
    //0x51
    // Processor mode can be usermode or kernel mode.
    KPROCESSOR_MODE ApcMode;
    //0x52
    BOOLEAN Inserted;
    // 0x53-58 padding because of allingment
} *KAPC;

typedef void* PKKERNEL_ROUTINE;
typedef void* PKRUNDOWN_ROUTINE;
typedef void* PKNORMAL_ROUTINE;

// First argument -> rcx
// Second argument -> rdx
// Third argument -> r8
// Forth argumnet -> r9
// Fifth argument -> rsp+0x28 (8 Bytes address of RIP, 32 Bytes of shadow space Assuming x64)
// Sixth arguemnt -> rsp+0x30
// Seventh Argument -> rps+0x38
// Eighth Argument -> rsp+0x40
void KeInitializeApc(
    KAPC Apc,
    KTHREAD Thread,
    KAPC_ENVIRONMENT Environment,
    PKKERNEL_ROUTINE KernelRoutine,
    PKRUNDOWN_ROUTINE RundownRoutine,
    PKNORMAL_ROUTINE NormalRoutine,
    KPROCESSOR_MODE ProcessorMode,
    PVOID NormalContext
) {
    Apc->Type = 0x12;
    Apc->Size = 0x58;
    if (Environment == CurrentApcEnvironment) {
        Apc->ApcStateIndex = Thread->ApcStateIndex; 
    } 
    else {
        Apc->ApcStateIndex = Environment;
    }
    Apc->Reserved[1] = RundownRoutine;
    Apc->Reserved[2] = NormalRoutine;
    Apc->Thread = Thread;

}