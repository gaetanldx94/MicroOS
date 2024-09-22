section .multiboot
    dd 0x1BADB002
    dd (1 << 0 | 1 << 1)
    dd -(0x1BADB002 + (1 << 0 | 1 << 1))

section .text
    global loader
    extern kernelMain
    extern idt_load
    extern load_page_directory
    extern enable_paging
    extern isr0
    extern isr1
    extern isr2
    extern isr3
    extern isr4
    extern isr5
    extern isr6
    extern isr7
    extern isr8
    extern isr9
    extern isr10
    extern isr11
    extern isr12
    extern isr13
    extern isr14
    extern isr15
    extern isr16
    extern isr17
    extern isr18
    extern isr19
    extern isr20
    extern isr21
    extern isr22
    extern isr23
    extern isr24
    extern isr25
    extern isr26
    extern isr27
    extern isr28
    extern isr29
    extern isr30
    extern isr31
    extern inb

loader:
    mov esp, 0x9FC00
    call kernelMain

_stop:
    cli
    hlt
    jmp _stop

idt_load:
    lidt [esp + 4]
    ret

load_page_directory:
    mov eax, [esp + 4]
    mov cr3, eax
    ret

enable_paging:
    mov eax, cr0
    or eax, 0x80000000
    mov cr0, eax
    ret

isr0: cli; pusha; call isr_handler; popa; sti; iret
isr1: cli; pusha; call isr_handler; popa; sti; iret
isr2: cli; pusha; call isr_handler; popa; sti; iret
isr3: cli; pusha; call isr_handler; popa; sti; iret
isr4: cli; pusha; call isr_handler; popa; sti; iret
isr5: cli; pusha; call isr_handler; popa; sti; iret
isr6: cli; pusha; call isr_handler; popa; sti; iret
isr7: cli; pusha; call isr_handler; popa; sti; iret
isr8: cli; pusha; call isr_handler; popa; sti; iret
isr9: cli; pusha; call isr_handler; popa; sti; iret
isr10: cli; pusha; call isr_handler; popa; sti; iret
isr11: cli; pusha; call isr_handler; popa; sti; iret
isr12: cli; pusha; call isr_handler; popa; sti; iret
isr13: cli; pusha; call isr_handler; popa; sti; iret
isr14: cli; pusha; call isr_handler; popa; sti; iret
isr15: cli; pusha; call isr_handler; popa; sti; iret
isr16: cli; pusha; call isr_handler; popa; sti; iret
isr17: cli; pusha; call isr_handler; popa; sti; iret
isr18: cli; pusha; call isr_handler; popa; sti; iret
isr19: cli; pusha; call isr_handler; popa; sti; iret
isr20: cli; pusha; call isr_handler; popa; sti; iret
isr21: cli; pusha; call isr_handler; popa; sti; iret
isr22: cli; pusha; call isr_handler; popa; sti; iret
isr23: cli; pusha; call isr_handler; popa; sti; iret
isr24: cli; pusha; call isr_handler; popa; sti; iret
isr25: cli; pusha; call isr_handler; popa; sti; iret
isr26: cli; pusha; call isr_handler; popa; sti; iret
isr27: cli; pusha; call isr_handler; popa; sti; iret
isr28: cli; pusha; call isr_handler; popa; sti; iret
isr29: cli; pusha; call isr_handler; popa; sti; iret
isr30: cli; pusha; call isr_handler; popa; sti; iret
isr31: cli; pusha; call isr_handler; popa; sti; iret

inb:
    mov dx, [esp + 4]
    in al, dx
    movzx eax, al
    ret

section .bss
    resb 2 * 1024 * 1024
