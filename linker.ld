ENTRY(loader)
OUTPUT_FORMAT(elf32-i386)
OUTPUT_ARCH(i386:i386)

SECTIONS {
    . = 0x00100000;

    .multiboot : {
        *(.multiboot)
    }

    .text : {
        *(.text)
    }

    .rodata : {
        *(.rodata*)
    }

    .data : {
        *(.data)
    }

    .bss : {
        *(COMMON)
        *(.bss)
    }
}
