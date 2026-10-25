# Libasm


## Compilation
nasm -f elf64 source.s -o code_objet.o

## Linkage
ld -m elf_x86_64 code_objet.o -o programme

gcc main.c code_objet.o -o programme

## Ecriture d'une fonction en asm appelable en C
En suivant la convention d'appel System V :

* Les arguements de la fonctions sont passés dans l'ordre dans les registres :
rdi, rsi, rdx, rcx, r8, r9 et sur la stack pour les suivants.

* Le retour de la fonction est ecrit dans le registre rax.

* La fonction doit préserver les registres rbx, rsp, rbp, r12, r13, r14, r15. Soit en n'y touchant pas soit en les suvegardant avant usage et en les restaurant avant de terminer.

# Instuctions utilisé
| mnemonique        | signification                      | action                                                           | Equivalence          |
| ----------------- | ---------------------------------- | ---------------------------------------------------------------- | -------------------- |
| mov rdx, rdi      | move destination, source           | copie le contenu du registre source dans le registre destination | destination = source |
| mov rdx, [adress] | move destination, valeur a adresse | copie dans destination la valeur contenu en memoire a adress     | registre = *adress   |
| test al, al       | teste registre, valeur             | efectue un AND logique                                           |                      |
| je label_equal    | jump egqual etiquette              | saute a létiquette si egal                                       |                      |

# Accès mémoire
Pour obtenir le contenu en mémoire, on utilise la notation `[adresse]`.
`adresse` peut être un registre, une valeur hexadécimale ou un calcul :
mov eax, [ebx]
mov eax, [0x8A26]
mov eax, [ebx + ecx*4 + 8]

Une exception à connaître pour l'instruction `lea destination, [adresse]` :
les crochets servent à calculer l'adresse, pas à récupérer le contenu.



# Astuces
`test al, al` plutot que `cmp al, 0`
