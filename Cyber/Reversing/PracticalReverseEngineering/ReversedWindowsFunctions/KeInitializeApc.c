typedef unsigned char UCHAR;
typedef unsigned long ULONG;
typedef void* PVOID;
typedef struct LIST_ENTRY {} LIST_ENTRY;
typedef char CCHAR;
typedef enum bool {true, fales} BOOLEAN;
typedef enum KPROCESSOR_MODE {kernel, user} KPROCESSOR_MODE;

typedef struct _KTHREAD {
    // info in ida
} KTHREAD;

// size=0x58
typedef struct KAPC { 
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
} KAPC;

void KeInitializeApc(
    KAPC Apc,
    KTHREAD Thread,
    KAPC_ENVIRONMENT TargetEnvironment,
) {

}