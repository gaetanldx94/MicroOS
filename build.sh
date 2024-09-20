# Create bin directory if it doesn't exist
mkdir -p bin

# Compile the Assembly File
nasm -f elf32 boot/boot.asm -o bin/boot.o

# Compile the C++ Kernel File
g++ -m32 -c kernel/kernel.cpp -o bin/kernel.o

# Link the Object Files
ld -m elf_i386 -T linker.ld -o bin/kernel.bin bin/boot.o bin/kernel.o

# Create ISO (optional)
mkdir -p iso/boot/grub
cp bin/kernel.bin iso/boot/
echo "set timeout=0" > iso/boot/grub/grub.cfg
echo "set default=0" >> iso/boot/grub/grub.cfg
echo "menuentry 'MicroOS' {" >> iso/boot/grub/grub.cfg
echo "    multiboot /boot/kernel.bin" >> iso/boot/grub/grub.cfg
echo "}" >> iso/boot/grub/grub.cfg
grub-mkrescue -o kernel.iso iso

qemu-system-i386 -cdrom kernel.iso -boot d