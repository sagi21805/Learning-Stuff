global __strlen
global __strchr
global __memcpy
global __memset
global __strcmp
global __strset

section .text

__strlen:
    ; first argument in RDI -> String Address
    push rbp
    mov rbp, rsp
    mov rsi, rdi               ; Save original pointer
    xor al, al                 ; Move the lowest byte of rdx to al
    mov rcx, -1                ; Maximum count (0xFFFFFFFFFFFFFFFF)
    repne scasb                ; Scan memory for byte in dl (second arg)
    sub rdi, rsi               ; Compute the length of the string (End address - Start Address)
    dec rdi                    ; Decrease length by 1 so the \0 is not included
    mov rax, rdi               ; Return value
    pop rbp
    ret

__strchr:
    ; first arguement in RDI -> String address
    ; second argument in RSI -> Wanted Character
    xor rax, rax               ; Zero out rax for any garbage value
    strchr_loop:
        mov al, byte [rdi]     ; Move current byte to al
        test al, al            ; if al == 0, string ends;
        jz strchr_end          ; if byte null string ends and wanted byte is not there
        cmp al, sil            ; sil in the wanted byte al is current byte;
        jz found_byte          ; if al - sil == 0 byte is found, exist
        inc rdi                
        jmp strchr_loop               ; if byte is not matching loop all over again
    found_byte:
        mov rax, rdi
    strchr_end:
        ret

__memcpy:
    ; first argument in RDI  -> Destintaion address (copy to) 
    ; second argument in RSI -> Source address (copy from)
    ; third argument in RDX  -> Size in bytes to copy

    mov r8, rdi            ; save start of dest address

    ; copy quad words (8 bytes) 
    mov rax, rdx           ; Copy size to dividened     
    mov rcx, 8             ; Copy 8 to the devisor
    xor rdx, rdx
    div rcx                ; RAX = Size // 8 | RDX = Size % 8 
    mov rcx, rax           ; Set loop counter to Size // 8
    rep movsq              ; Copy quad words

    ; Copy double words (4 bytes)
    mov rax, rdx           ; Copy remaining Size to dividend
    mov rcx, 4             ; Copy 4 to the devisor
    xor rdx, rdx
    div rcx                ; RAX = Size // 4 | RDX = Size % 4
    mov rcx, rax           ; Set loop counter to remaining size // 4
    rep movsd              ; Copy double words
    
    ; Copy words (2 bytes)
    mov rax, rdx           ; Copy remaining Size to dividend
    mov rcx, 2             ; Copy 2 to the divisor 
    xor rdx, rdx
    div rcx                ; RAX = Size // 2 | RDX = Size % 2
    mov rcx, rax           ; Set loop counter to remaining size // 2
    rep movsw              ; Copy words
   
    ; Copy remaining bytes
    mov rcx, rdx          ; Copy remaining reminder to loop counter     
    rep movsb             ; Copy single bytes
    
    mov rax, r8           ; load start address to rax
    retn


__memset:
    ; first argument in RDI  -> Address to write to 
    ; second argument in RSI -> Value to write
    ; third argument in RDX  -> Size in bytes to write
    xor rax, rax
    mov r8, rdi
    mov al, sil            ; Move the first byte of RSI to al
    mov rcx, rdx           ; Set counter to number of bytes
    rep stosb              ; Fill RDI with the value in al
    mov rax, r8
    retn

__strcmp:
    ; first argument in RDI  -> First string
    ; second argument in RSI -> Second String

    xor rax, rax               ; Zero out rax for any garbage value
    strcmp_loop:
        mov dl, byte [rdi]     ; Move current byte to al
        mov cl, byte [rsi]
        test dl, dl            ; if dl == 0, string ends;
        jz strcmp_end          ; if byte null string ends and wanted byte is not there
        test cl, cl            ; if cl == 0, string ends;
        jz strcmp_end          ; if byte null string ends and wanted byte is not there
        sub cl, dl             ; check if cl == dl
        jnz not_matching       ; if cl - dl != 0 bytes are not matching, exist
        inc rdi               
        inc rsi            
        jmp strcmp_loop        ; if byte is not matching loop all over again
    not_matching:
        mov al, cl
    strcmp_end:
        retn

__strset:
    ; first argument in RDI -> String to change
    ; Second argument in RSI -> value to set

    xor rcx, rcx               ; Zero out any grabage value in rcx
    mov rax, rdi               ; Save start address of string
    strset_loop:
        mov cl, [rdi]          ; Move first byte to cl
        test cl, cl            ; Check End of string
        jz strset_end          ; If string ends return
        mov [rdi], sil         ; Set the value to the string byte
        inc rdi                
        jmp strset_loop
    strset_end:
        retn 
