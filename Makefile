# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: svogrig < svogrig@studebt.42.fr>           +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/10/24 11:46:07 by svogrig           #+#    #+#              #
#    Updated: 2026/10/26 16:25:30 by svogrig          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

MAKEFLAGS 		+= --no-print-directory

BLINK_GREEN		:=	\033[5;32m
BGREEN			:=	\033[1;32m
BHYEL			:= 	\033[1;93m
BOLD			:=	\033[1m
NC				:=	\033[0m

NAME_LIB 		:= libasm.a
NAME_TEST		:= test_libasm

# includes --------------------------------------------------------------------#

DIR_INC				:=	include

I_FLAG				:=	$(addprefix -I,$(DIR_INC)) -MMD -MP

# sources ---------------------------------------------------------------------#

DIR_SRC			:=	src

SRC_ASM			:= ft_strlen.s

SRC_C			:= main.c

# objects ---------------------------------------------------------------------#

OBJ_DIR			:=	.build
OBJ_ASM     	:= $(addprefix $(OBJ_DIR)/, $(SRC_ASM:.s=.o))
OBJ_C			:= $(addprefix $(OBJ_DIR)/, $(SRC_C:.c=.o))

# compilation -----------------------------------------------------------------#

NASM			:= nasm
NASM_FLAGS 		:= -f elf64

AR				:=	ar
ARFLAGS			:=	-r -c -s

CC				:=	cc
CC_FLAGS		:=	-Wall -Wextra -Werror

#------------------------------------------------------------------------------#
# rules                                                                        #
#------------------------------------------------------------------------------#

all: init
	@$(MAKE) -j make_libasm

clean:
	@rm -rf $(OBJ_DIR) && printf "Cleaning : $(OBJ_DIR)\n"

fclean: clean
	@rm -f $(NAME_LIB)
	@rm -rf $(OBJ_DIR) && printf "Cleaning : $(NAME_LIB)\n"

re: fclean
	@$(MAKE) -j all

init:
	@printf "\n"
	@printf "\t\t$(BHYEL)┍━━━━━━━━»•» 🌸 «•«━┑$(NC)\n"
	@printf "\t\t$(BHYEL)  libasm COMPILATION $(NC)\n"
	@printf "\t\t$(BHYEL)┕━»•» 🌸 «•«━━━━━━━━┙$(NC)\n"
	@printf "\n"

test: $(NAME_TEST)

make_libasm: $(NAME_LIB)
	@printf "\n"
	@printf "\t$(BLINK_GREEN)$(NAME_LIB) = COMPILATION FINISHED !$(NC)\n"

#------------------------------------------------------------------------------#
# compilation                                                                  #
#------------------------------------------------------------------------------#

$(OBJ_DIR)/%.o: $(DIR_SRC)/%.s
	@mkdir -p $(@D)
	@printf "\033[2K\rcompiling : %s\n" $@
	@$(NASM) $(NASM_FLAGS) $< -o $@

$(OBJ_DIR)/%.o: $(DIR_SRC)/%.c
	@mkdir -p $(@D)
	@printf "\033[2K\rcompiling : %s\n" $@
	@$(CC) $(CC_FLAGS) $(I_FLAG) -c $< -o $@

#------------------------------------------------------------------------------#
# linkage                                                                      #
#------------------------------------------------------------------------------#

$(NAME_LIB): $(OBJ_ASM)
	@$(AR) $(ARFLAGS) $(NAME_LIB) $(OBJ_ASM)
	@printf "\033[2K\rcreated   : %s\n" $@

$(NAME_TEST): $(OBJ_C) $(NAME_LIB)
	@$(CC) $(CC_FLAGS) $^ -o $@ && printf "$(BGREEN)\tLinking Exe:\t$(BOLD)$@$(NC)\n"

#------------------------------------------------------------------------------#
# specifications                                                               #
#------------------------------------------------------------------------------#

.PHONY: all \
		clean \
		fclean \
		bonus \
		re \
		makeall
