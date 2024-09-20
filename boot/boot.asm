; boot.asm
section .multiboot
    dd 0x1BADB002         ; Magic number
    dd (1 << 0 | 1 << 1)  ; Flags
    dd -(0x1BADB002 + (1 << 0 | 1 << 1))  ; Checksum

section .text
    global loader
    extern kernelMain

loader:
    ; Set up the stack
    mov esp, 0x9FC00      ; Example stack pointer location

    ; Call the kernel main function
    call kernelMain

    ; Halt the CPU
_stop:
    cli                   ; Disable interrupts
    hlt                   ; Halt the CPU
    jmp _stop             ; Loop forever

section .bss
    resb 2 * 1024 * 1024  ; Reserve 2MB for kernel stack
