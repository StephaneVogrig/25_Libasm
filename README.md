# Libasm

Ce projet post tronc commun de l'école 42 a pour but de découvrir la programmation bas niveau en assembleur. Il consiste en la réalisation d'une librairie de commandes de base utilisables ensuite par un programme en C :
- ft_strlen
- ft_strcpy
- ft_strcmp
- ft_write
- ft_read
- ft_strdup
- ft_calloc

## Syntaxe
Il existe deux principales syntaxes pour l'écriture assembleur : Intel et AT&T.
Pour ce projet, il est demandé d'utiliser la syntaxe Intel avec le compilateur NASM.

[Intel and AT&T Syntax](https://imada.sdu.dk/u/kslarsen/dm546/Material/IntelnATT.htm)
[OSDev Wiki - Assembly](https://wiki.osdev.org/Assembly)

## Compilation
Pour compiler un fichier source en asm :
```bash
nasm -f elf64 source.s -o code_objet.o
```
Pour étudier le résultat de la compilation d'un programme en C :
```bash
gcc -O2 -S -masm=intel -fcf-protection=none -fno-asynchronous-unwind-tables test.c -o test.s
```
## Linkage
Pour créer un exécutable :
* Avec `ld`
    ```bash
    ld -m elf_x86_64 code_objet.o -o programme
    ```
    Le fichier .o doit :
    - définir l'étiquette `_start`
    - exporter `_start` avec `global _start`
    - terminer par un `syscall exit`
        ```nasm
        xor edi, edi    ; rdi = code de sortie (0 = succès)
        mov eax, 60     ; sys_exit
        syscall
        ```

* Avec `gcc`
    ```bash
    gcc main.c code_objet.o -o programme
    ```
    gcc lie automatiquement le code de démarrage de la libc (`crt1.o`), qui définit `_start` et appelle `main`. Le .o n’a donc pas à définir `_start` mais une fonction `main` doit exister dans l'un des fichiers liés.

## Bibliothèque
Créer l'archive :
```bash
ar rcs libasm.a ft_strlen.o ft_strcpy.o ...
```

Compiler le main de test avec la lib :
```bash
gcc -Wall -Wextra -Werror main.c -L. -lasm -o test
```

## Structure d’un programme NASM
### Organisé en sections :
* .data : contient les définitions des variables initialisées.
* .bss : contient les définitions des variables non-initialisées.
* .text : contient le code.
    Le mot-clé `section` en début de ligne démarre la section, les sections se suivent sans autre délimiteur.
    ```nasm
    section .data
        ...
    section .bss
        ...
    section .text
        ...
    ```
### Commentaires
Le `;` signale que la suite de la ligne est un commentaire et est ignorée par le compilateur (comme `//` en C).

### Labels
Dans le corps du programme, des labels sont utilisés pour créer des points d'accès nommés, et sont constitués d'un nom suivi de `:`.

## Écriture d'une fonction en asm appelable en C
En suivant la convention d'appel System V :

* Les arguments de la fonction sont passés dans l'ordre dans les registres :
`rdi`, `rsi`, `rdx`, `rcx`, `r8`, `r9` et sur la stack pour les suivants.

* Le retour de la fonction est écrit dans le registre `rax`.

* La fonction doit préserver :
    - les registres `rbx`, `rbp`, `r12`, `r13`, `r14`, `r15`. Soit en n'y touchant pas, soit en les sauvegardant avant usage et en les restaurant avant de terminer.
    - la pile `rsp` doit être restaurée par l’équilibre push/pop.

* La fonction n'a pas besoin de restaurer les registres `rax`, `rcx`, `rdx`, `rsi`, `rdi`, `r8`, `r9`, `r10`, `r11`.

* `r10` et `r11` : temporaires purs

* Signification/usage historique des registres :
    - `rax` : Accumulateur
    - `rcx` : Compter
    - `rdx` : Data
    - `rsp` : Stack Pointer
    - `rip` : Instruction Pointer
    - `rbp` : Base Pointer
    - `rsi` : Source Index
    - `rdi` : Destination Index

## Instructions utilisées
| mnémonique        | signification                      | action                                                                      | Équivalence            |
| ----------------- | ---------------------------------- | --------------------------------------------------------------------------- | ---------------------- |
| mov rdx, rdi      | move destination, source           | copie le contenu du registre source dans le registre destination            | destination = source   |
| mov rdx, [adress] | move destination, valeur à adresse | copie dans destination la valeur contenue en mémoire à adresse              | registre = *adress     |
| test al, al       | teste registre, valeur             | effectue un AND sans modifier les registres, mais modifie les flags logique | if (al == 0)           |
| je label_equal    | jump equal étiquette               | saute si le flag ZF = 1                                                     | if (ZF == 1) goto label |

* [movzx](https://www.reverseengineering.app/en/assembly/movzx-movsx)
    ```nasm
    movzx eax, bl   ; copie bl dans eax en remplissant les bits 8 à 31 avec des zéros
                    ; (les 32 bits hauts de rax sont aussi mis à zéro automatiquement)
    ```

    Équivalent à :
    ```nasm
    xor eax, eax
    mov al, bl
    ```

    Attention :
    - `xor` modifie les flags alors que `movzx` ne les modifie pas
    - l'équivalence n'est valable que si la source n'est pas incluse
    dans la destination (`movzx eax, al` ≠ `xor eax, eax` + `mov al, al`).
    Dans ce cas, utiliser `and eax, 0xFF`, `and` modifie les flags.


## Accès mémoire
Pour obtenir le contenu en mémoire, on utilise la notation `[...]`.
Le contenu peut être un registre ou un calcul :
```nasm
mov rax, [rbx]
mov rax, [rbx + rcx*4 + 8]
```
Exception à connaître, où les crochets servent à calculer l'adresse, pas à récupérer le contenu :

```nasm
lea rax, [rdi + rcx*4]         ; rax = rdi + rcx*4, aucun accès mémoire
```
`lea` = load effective address.

Dans un accès mémoire, le calcul `[...]` est fait par l'AGU (Address Generation Unit), pas par l'ALU (Arithmetic Logic Unit). Cela évite une instruction `add` séparée et ne mobilise pas de port ALU, mais le calcul a une latence propre, incluse dans celle du load. `[rdi + rax]` revient donc moins cher qu'un `add` suivi d'un `mov`, surtout en nombre d’instructions et de registres utilisés.
Pour `lea`, l'unité utilisée dépend du CPU (AGU sur les anciens, souvent ALU sur les Intel récents).

### Accés indépendant de la position

| Sigle | Nom complet | Rôle |
|---|---|---|
| **PIC** | Position Independent Code | Code sans adresse absolue, exécutable à n'importe quelle adresse |
| **PIE** | Position Independent Executable | Exécutable entièrement compilé/lié en PIC |
| **ASLR** | Address Space Layout Randomization | Randomisation des adresses (pile, tas, libs, binaire PIE) à chaque exécution |
| **ROP** | Return-Oriented Programming | Attaque chaînant des « gadgets » de code existant (rendue plus difficile par l'ASLR) |
| **NX / DEP** | No eXecute / Data Execution Prevention | Pile et tas non exécutables |
| **RELRO** | RELocation Read-Only | Rend la GOT (partiellement ou totalement) en lecture seule après chargement |
| **PLT** | Procedure Linkage Table | Stubs de code : `call malloc@PLT` saute vers l'adresse stockée dans la GOT |
| **GOT** | Global Offset Table | Tableau de pointeurs rempli par le loader avec les vraies adresses des symboles externes |
| **ELF** | Executable and Linkable Format | Format des binaires sous Linux |
| **EXEC / DYN** | Types ELF | `EXEC` = non-PIE (adresse fixe), `DYN` = PIE ou bibliothèque partagée |
| **ABI** | Application Binary Interface | Convention d'appel (ici System V AMD64) |

## Astuces
* `xor reg, reg` plutôt que `mov reg, 0` :<br>
Met le registre à zéro sans encoder de valeur immédiate, ce qui donne un code machine plus compact de 3 à 4 octets. Attention : `xor` modifie les flags, contrairement à `mov`. Idiome standard pour mettre un registre à zéro. `xor eax, eax` met à zéro tout rax grâce à l’extension à zéro automatique des écritures 32 bits, c’est la forme préférée à `xor rax, rax`.
* `test reg, reg` plutôt que `cmp reg, 0` :<br>
Cela évite d'encoder une valeur immédiate, ce qui donne un code machine plus compact de 1 octet.
Il n'y a pas de différence de vitesse sur les processeurs modernes, mais c'est l'idiome standard pour tester la nullité d'un registre ([Stack Overflow : in x86 whats difference between test eax, eax and cmp eax, 0](https://stackoverflow.com/questions/39556649/in-x86-whats-difference-between-test-eax-eax-and-cmp-eax-0)).
Pour un octet en mémoire (`ft_strlen`), `cmp byte [rax], 0` reste la bonne forme.

## Débug
* Désassembler un .o : objdump -d -M intel ft_strlen.o

* Executer pas à pas:
```bash
gdb ./test
```
puis, dans gdb : layout asm, layout regs, break ft_strlen, stepi, info registers.

* Vérifier les appels système :
```bash
strace ./test
```
* Verifier les fuites mémoires :
```bash
valgrind --leak-check=full ./test
```



## Documentation
[x86 Assembly/Interfacing with Linux](https://en.wikibooks.org/wiki/X86_Assembly/Interfacing_with_Linux)<br>
[Optimizing subroutines in assembly language](https://www.agner.org/optimize/optimizing_assembly.pdf)<br>
