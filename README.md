# Libasm


## Compilation
nasm -f elf64 source.s -o code_objet.o

## Linkage
ld -m elf_x86_64 code_objet.o -o programme

gcc main.c code_objet.o -o programme