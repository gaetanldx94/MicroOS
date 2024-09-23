mkdir -p bin

nasm -f elf32 boot/boot.asm -o bin/boot.o

g++ -m32 -fno-stack-protector -c kernel/kernel.cpp -o bin/kernel.o
g++ -m32 -fno-stack-protector -c kernel/memory.cpp -o bin/memory.o
g++ -m32 -fno-stack-protector -c kernel/process.cpp -o bin/process.o
g++ -m32 -fno-stack-protector -c kernel/file.cpp -o bin/file.o
g++ -m32 -fno-stack-protector -c kernel/string_utils.cpp -o bin/string_utils.o
g++ -m32 -fno-stack-protector -c kernel/interrupts.cpp -o bin/interrupts.o
g++ -m32 -fno-stack-protector -c kernel/keyboard.cpp -o bin/keyboard.o
g++ -m32 -fno-stack-protector -c kernel/paging.cpp -o bin/paging.o
g++ -m32 -fno-stack-protector -c kernel/shell.cpp -o bin/shell.o
g++ -m32 -fno-stack-protector -c kernel/video.cpp -o bin/video.o

ld -m elf_i386 -T linker.ld -o bin/kernel.bin bin/boot.o bin/kernel.o bin/memory.o bin/process.o bin/file.o bin/string_utils.o bin/interrupts.o bin/keyboard.o bin/paging.o bin/shell.o bin/video.o

mkdir -p iso/boot/grub
cp bin/kernel.bin iso/boot/
echo "set timeout=0" > iso/boot/grub/grub.cfg
echo "set default=0" >> iso/boot/grub/grub.cfg
echo "menuentry 'MicroOS' {" >> iso/boot/grub/grub.cfg
echo "    multiboot /boot/kernel.bin" >> iso/boot/grub/grub.cfg
echo "}" >> iso/boot/grub/grub.cfg
grub-mkrescue -o MicroOS.iso iso

qemu-system-i386 -cdrom MicroOS.iso -boot d
