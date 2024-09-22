mkdir -p bin

nasm -f elf32 boot/boot.asm -o bin/boot.o

g++ -m32 -fno-stack-protector -c kernel/kernel.cpp -o bin/kernel.o
g++ -m32 -fno-stack-protector -c kernel/memory.cpp -o bin/memory.o
g++ -m32 -fno-stack-protector -c kernel/process.cpp -o bin/process.o
g++ -m32 -fno-stack-protector -c kernel/file.cpp -o bin/file.o
g++ -m32 -fno-stack-protector -c kernel/string_utils.cpp -o bin/string_utils.o

ld -m elf_i386 -T linker.ld -o bin/kernel.bin bin/boot.o bin/kernel.o bin/memory.o bin/process.o bin/file.o bin/string_utils.o

mkdir -p iso/boot/grub
cp bin/kernel.bin iso/boot/
echo "set timeout=0" > iso/boot/grub/grub.cfg
echo "set default=0" >> iso/boot/grub/grub.cfg
echo "menuentry 'MicroOS' {" >> iso/boot/grub/grub.cfg
echo "    multiboot /boot/kernel.bin" >> iso/boot/grub/grub.cfg
echo "}" >> iso/boot/grub/grub.cfg
grub-mkrescue -o MicroOS.iso iso

qemu-system-i386 -cdrom MicroOS.iso -boot d
