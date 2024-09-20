ENTRY(loader)
OUTPUT_FORMAT(elf32-i386)
OUTPUT_ARCH(i386:i386)

SECTIONS {
    . = 0x00100000;  /* Code starts at 1MB */

    .multiboot : {
        *(.multiboot)  /* Include the Multiboot section */
    }

    .text : {
        *(.text)      /* All .text sections */
    }

    .rodata : {
        *(.rodata*)   /* All .rodata sections */
    }

    .data : {
        *(.data)      /* All .data sections */
    }

    .bss : {
        *(COMMON)     /* All COMMON sections */
        *(.bss)       /* All .bss sections */
    }
}
