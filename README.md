# Libasm

Ce projet post tronc commun de l'ecole 42, a pour but de découvrir la programmation bas niveau en assembleur. Il consiste en la réalisation d'une librairie de commandes de bases utilisable ensuite par un programme en C :
- ft_strlen
- ft_strcpy
- ft_strcmp
- ft_write
- ft_read
- ft_strdup
- ft_calloc

## Syntaxe
Il existe deux principales syntaxe pour l'écriture assembleur : Intel et AT&T.
Pour ce projet il est demandé d'utiliser la syntaxe Intel avec le compilateur NASM.

[Intel and AT&T Syntax](https://imada.sdu.dk/u/kslarsen/dm546/Material/IntelnATT.htm)
[OSDev Wiki - Assembly](https://wiki.osdev.org/Assembly)


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
Pour obtenir le contenu en mémoire, on utilise la notation `[...]`.
Le contenu peut être un registre, une valeur hexadécimale ou un calcul :
```basm
mov eax, [ebx]
mov eax, [0x8A26]
mov eax, [ebx + ecx*4 + 8]
```
Une exception à connaître ou les crochets servent à calculer l'adresse, pas à récupérer le contenu :

```basm
lea destination, [adresse]
```

# Astuces
* `test reg, reg` plutôt que `cmp reg, 0` :<br>
Cela évite d'encoder une valeur immédiate, ce qui donne un code machine plus compact de 1 octet.
Il n'y a pas de différence de vitesse de traitement sur les processeurs modernes, mais c'est l'idiome standard pour tester la nullité d'un registre ([Stackoverflow - Difference entre test et cmp](https://stackoverflow.com/questions/39556649/in-x86-whats-difference-between-test-eax-eax-and-cmp-eax-0)).

# Documentation
[x86 Assembly/Interfacing with Linux](https://en.wikibooks.org/wiki/X86_Assembly/Interfacing_with_Linux)<br>
[Optimizing subroutines in assembly language](https://www.agner.org/optimize/optimizing_assembly.pdf)<br>
