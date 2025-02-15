 union _KDPC_Union1 {                                       // XREF: sub_1402E8960+392/w
                                        // sub_14027DA20+1C4066/w ...
    ULONG TargetInfoAsUlong;
    struct _KDPC_Union2;
};
struct _KDPC_Union2 {                                       // XREF: _KDPC::$6963FFE55BC4627E7AE37E9650A61BA0/r
    UCHAR Type;
    UCHAR Importance;
    volatile USHORT Number;
};

typedef struct _KDPC { // size=0x40

    union _KDPC                 
    // 0x0-0x8
    SINGLE_LIST_ENTRY  DpcListEntry
    // 0x0-0x10
    // This is a pointer to a LinkedList entry
    KAFFINITY          ProcessorHistory
    // 0x0-0x18
    // Mask that signals wich CPU was used
    // 00000100 -> third cpu was used
    PKDEFERRED_ROUTINE DeferredRoutine
    // Pointer to the routine function, which takes the bellow arguments
    //
    PVOID              DeferredContext
    PVOID              SystemArgument1
    PVOID              SystemArgument2    

    PVOID              DpcData
}