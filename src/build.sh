nasm -felf32 boot.s -o boot.o
gcc -c kernel.c -o kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
gcc -ffreestanding -nostdlib -fno-use-linker-plugin -g -T linker.ld boot.o kernel.o -o rise.elf -lgcc

