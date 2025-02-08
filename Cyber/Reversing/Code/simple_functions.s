global __strlen
global __strchr
global __memcpy
; global __memset
; global __strcmp
; global __set

section .text

__strlen:
    push rbx                   ; Preserve rbx (callee-saved)
    push rdi                   ; Preserve rdi (callee-saved)
    mov rdi, rcx               ; First argument (pointer to string)
    mov rbx, rdi               ; Save original pointer
    xor al, al                 ; Move the lowest byte of rdx to al
    mov rcx, -1                ; Maximum count (0xFFFFFFFFFFFFFFFF)
    repne scasb                ; Scan memory for byte in dl (second arg)
    sub rdi, rbx               ; Compute dl byte place + 1;
    dec rdi                    ; Decrease length by 1 so the \0 is not included
    mov rax, rdi               ; Return value
    pop rdi                    ; Restore rdi
    pop rbx                    ; Restore rbx
    ret

__strchr:
    xor rax, rax               ; Zero out rax for any garbage value
    loop:
        mov al, byte [rcx]     ; Move current byte to al
        test al, al            ; if al == 0, string ends;
        jz string_end          ; if byte null string ends and wanted byte is not there
        cmp al, dl             ; dl in the wanted byte bl is current byte;
        jz found_byte          ; if al - dl == 0 byte is found, exist
        inc rcx                ; increase rcx by one
        jmp loop               ; if byte is not matchin loop all over again
    found_byte:
        mov rax, rcx
    string_end:
        ret

__memcpy:
    ; rcx copy to, rdx copy from, r8 size  
    mov rsi, rdx               
    mov rdi, rcx
    xor rcx, rcx
    xor rax, rax
    xor rdx, rdx
    ; set rsi and rdx to source and index pointers
    mov rax, r8     
    mov rcx, 8  
    ; set dividend in rax and divisor in rcx  
    xor rdx, rdx
    div rcx                    
    ; Stores rax // 8 in rax, and rax % 8 in rdx 
    mov rcx, rax               
    rep movsq                  
    ; move quotient to counter and copy quad words rcx times
    mov rax, rdx       
    xor rdx, rdx       
    mov rcx, 4
    ; move current reminder to next dividend and set divisor to 4
    div rcx                    
    ; Stores rax // 4 in rax, and rax % 4 in rdx
    mov rcx, rax              
    rep movsd                 
    ; move quotient to counter and copy double words rcx times
    mov rax, rdx               
    xor rdx, rdx
    mov rcx, 2
    ; move current reminder to next dividend and set divisor to 2
    div rcx                   
    ; Stores rax // 2 in rax, and rax % 2 in rdx 
    mov rcx, rax               
    rep movsw                  
    ; move quotient to counter and copy words rcx times
    mov rcx, rdx               
    rep movsb                  
    ; move quotient to counter and copy byte rcx times
    neg r8       
    lea rax, [rdi + r8]
    ; return the destination address in rax
    ret

__memset:
    mov rsi, rdx
    mov rdi, rcx
    xor rcx, rcx
    xor rax, rax
    xor rdx, rdx
    mov rax, r8
    mov rcx, 8
    xor rdx, rdx
    div rcx
    mov rcx, rax
    rep movsq
    mov rax, rdx
    xor rdx, rdx
    mov rcx, 4
    div rcx
    mov rcx, rax
    rep movsd
    mov rax, rdx
    xor rdx, rdx
    mov rcx, 2
    div rcx
    mov rcx, rax
    rep movsw
    mov rcx, rdx
    rep movsb
    neg r8
    lea rax, [rdi + r8]
    ret
