# Create bin directory if it doesn't exist
mkdir -p bin

# Compile the Assembly File
nasm -f elf32 boot/boot.asm -o bin/boot.o

# Compile the C++ Kernel File
g++ -m32 -fno-stack-protector -c kernel/kernel.cpp -o bin/kernel.o

# Compile the Memory Management Files
g++ -m32 -fno-stack-protector -c kernel/memory.cpp -o bin/memory.o

# Compile the Process Management Files
g++ -m32 -fno-stack-protector -c kernel/process.cpp -o bin/process.o

# Compile the Directory Management Files
g++ -m32 -fno-stack-protector -c kernel/file.cpp -o bin/file.o

# Compile the Utils Files
g++ -m32 -fno-stack-protector -c kernel/string_utils.cpp -o bin/string_utils.o

# Link the Object Files
ld -m elf_i386 -T linker.ld -o bin/kernel.bin bin/boot.o bin/kernel.o bin/memory.o bin/process.o bin/file.o bin/string_utils.o

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
