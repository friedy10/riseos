/home/professor/Documents/riseos/barebones-toolchain/setenv.sh
nasm -felf32 boot.s -o boot.o
i686-elf-gcc -c kernel.c -o kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
i686-elf-gcc -ffreestanding -nostdlib -fno-use-linker-plugin -g -T linker.ld boot.o kernel.o -o rise.elf -lgcc

