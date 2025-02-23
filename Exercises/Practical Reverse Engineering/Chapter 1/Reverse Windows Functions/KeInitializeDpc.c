typedef unsigned char UCHAR;
typedef unsigned long ULONG;
typedef unsigned short USHORT;
typedef void* SINGLE_LIST_ENTRY;
typedef ULONG KAFFINITY;
typedef void* PVOID; 
typedef void* PKDEFERRED_ROUTINE;

typedef union KDPC {                                   
    ULONG TargetInfoAsUlong;
    struct KDPCEX;
};

typedef struct KDPCEX {                                       
    UCHAR Type;
    UCHAR Importance;
    volatile USHORT Number;
};

// size=0x40
typedef struct _KDPC { 
    union KDPC Info; // 0x0-0x8

    // 0x8-0x10
    // This is a pointer to a LinkedList entry
    SINGLE_LIST_ENTRY DpcListEntry;
    
    // 0x10-0x18
    // Mask that signals wich CPU was used
    // 00000100 -> third cpu was used
    KAFFINITY ProcessorHistory;

    //0x18-20
    // Pointer to the routine function, which takes the bellow arguments
    PKDEFERRED_ROUTINE DeferredRoutine;
    
    //0x20-28
    PVOID DeferredContext;
    //0x28-30
    PVOID SystemArgument1;
    //0x30-38
    PVOID SystemArgument2;   
    //0x38-40
    PVOID DpcData;

} *PRKDPC;


// first argument -> rcx
// second argument -> rdx
// third arguemnt -> r8
void KeInitializeDpc(PRKDPC dpc, PKDEFERRED_ROUTINE DeferredRoutine, PVOID DeferredContext) {
    dpc->Info.TargetInfoAsUlong = 0x113ul;
    dpc->DpcData = 0;
    dpc->ProcessorHistory = 0;
    dpc->DeferredRoutine = DeferredRoutine;
    dpc->DeferredContext = DeferredContext;
    return 0;
}