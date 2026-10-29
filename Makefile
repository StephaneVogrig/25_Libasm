# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: svogrig < svogrig@studebt.42.fr>           +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/10/24 11:46:07 by svogrig           #+#    #+#              #
#    Updated: 2026/10/29 01:44:27 by svogrig          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

GREEN			:=	\033[32m
BOLD_GREEN		:=	\033[1;32m
NC				:=	\033[0m

LIB_NAME 		:=	libasm.a
TEST_NAME		:=	test_libasm

# includes --------------------------------------------------------------------#

DIR_INC			:=	include
CPPFLAGS 		:=	$(addprefix -I,$(DIR_INC))
DEP_FLAGS		:=	-MMD -MP

# sources ---------------------------------------------------------------------#

DIR_SRC			:=	src
SRC_ASM			:=	ft_strlen.s \
					ft_strcpy.s \
					ft_strcmp.s \
					ft_write.s \
					ft_read.s \
					ft_strdup.s \
					ft_calloc.s

SRC_C			:=	main.c

# objects ---------------------------------------------------------------------#

OBJ_DIR			:=	.build
OBJ_ASM			:=	$(addprefix $(OBJ_DIR)/, $(SRC_ASM:.s=.o))
OBJ_C			:=	$(addprefix $(OBJ_DIR)/, $(SRC_C:.c=.o))

# compilation -----------------------------------------------------------------#

NASM			:=	nasm
NASM_FLAGS 		:=	-f elf64

AR				:=	ar
ARFLAGS			:=	rcs

CC				:=	cc
CFLAGS			:=	-Wall -Wextra -Werror

#------------------------------------------------------------------------------#
# rules                                                                        #
#------------------------------------------------------------------------------#

all: lib

clean:
	@rm -rf $(OBJ_DIR) && printf "Cleaned : $(OBJ_DIR)\n"

fclean: clean
	@rm -f $(LIB_NAME) && printf "Cleaned : $(LIB_NAME)\n"
	@rm -f $(TEST_NAME) && printf "Cleaned : $(TEST_NAME)\n"

re: fclean all

test: $(TEST_NAME)

lib: $(LIB_NAME)

run: test
	@./$(TEST_NAME)

-include $(OBJ_C:.o=.d)

#------------------------------------------------------------------------------#
# compilation                                                                  #
#------------------------------------------------------------------------------#

$(OBJ_DIR)/%.o: $(DIR_SRC)/%.s
	@mkdir -p $(@D)
	@printf "Compiling : %s ... " $@
	@$(NASM) $(NASM_FLAGS) $< -o $@
	@printf "$(GREEN)done$(NC)\n"

$(OBJ_DIR)/%.o: $(DIR_SRC)/%.c
	@mkdir -p $(@D)
	@printf "Compiling : %s ... " $@
	@$(CC) $(CFLAGS) $(CPPFLAGS) $(DEP_FLAGS) -c $< -o $@
	@printf "$(GREEN)done$(NC)\n"

#------------------------------------------------------------------------------#
# linkage                                                                      #
#------------------------------------------------------------------------------#

$(LIB_NAME): $(OBJ_ASM)
	@$(AR) $(ARFLAGS) $(LIB_NAME) $(OBJ_ASM)
	@printf "$(BOLD_GREEN)Created   : %s$(NC)\n" $@

$(TEST_NAME): $(LIB_NAME) $(OBJ_C)
	@$(CC) $(CFLAGS) $(OBJ_C) $(LIB_NAME) -o $@ && printf "$(BOLD_GREEN)Created   : $@$(NC)\n"

#------------------------------------------------------------------------------#
# specifications                                                               #
#------------------------------------------------------------------------------#

.PHONY: all \
		clean \
		fclean \
		re \
		lib \
		test \
		run
