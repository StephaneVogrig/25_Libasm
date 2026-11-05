# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: svogrig <svogrig@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/10/24 11:46:07 by svogrig           #+#    #+#              #
#    Updated: 2026/11/05 17:42:26 by svogrig          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

GREEN			:=	\033[32m
BOLD_GREEN		:=	\033[1;32m
NC				:=	\033[0m

LIB_NAME 		:=	libasm.a
TEST_NAME		:=	test_libasm
TEST_DBG_NAME	:=	test_dbg

# includes --------------------------------------------------------------------#

DIR_INC			:=	include
CPPFLAGS 		:=	$(addprefix -I,$(DIR_INC))
DEP_FLAGS		:=	-MMD -MP

# sources ---------------------------------------------------------------------#

DIR_SRC			:=	src
DIR_SRC_ASM		:=  asm
DIR_SRC_TEST	:=  test
SRC_ASM			:=	ft_strlen.s \
					ft_strcpy.s \
					ft_strcmp.s \
					ft_write.s \
					ft_read.s \
					ft_strdup.s \
					ft_calloc.s
					
SRC_ASM			:=  $(addprefix $(DIR_SRC_ASM)/, $(SRC_ASM))

SRC_TEST		:=	main.c \
					display.c \
					utils.c \
					tests_ft_strlen.c \
					tests_ft_strcpy.c \
					tests_ft_strcmp.c \
					tests_ft_write.c \
					tests_ft_read.c \
					tests_ft_strdup.c \
					tests_ft_calloc.c

SRC_TEST		:=  $(addprefix $(DIR_SRC_TEST)/, $(SRC_TEST))

# objects ---------------------------------------------------------------------#

OBJ_DIR			:=	.build
OBJ_ASM			:=	$(addprefix $(OBJ_DIR)/, $(SRC_ASM:.s=.o))
OBJ_TEST		:=	$(addprefix $(OBJ_DIR)/, $(SRC_TEST:.c=.o))

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
	@rm -f $(TEST_DBG_NAME) && printf "Cleaned : $(TEST_DBG_NAME)\n"

re: fclean all

test: $(TEST_NAME)

lib: $(LIB_NAME)

run: test
	@./$(TEST_NAME)

debug: $(TEST_DBG_NAME)

valgrind: $(TEST_NAME)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(TEST_NAME)

-include $(OBJ_TEST:.o=.d)

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

$(TEST_NAME): $(LIB_NAME) $(OBJ_TEST)
	@$(CC) $(CFLAGS) $(OBJ_TEST) $(LIB_NAME) -o $@ && printf "$(BOLD_GREEN)Created   : $@$(NC)\n"

$(TEST_DBG_NAME): $(LIB_NAME) $(OBJ_TEST)
	@$(CC) $(CFLAGS) -g $(OBJ_TEST) $(LIB_NAME) -o $@ && printf "$(BOLD_GREEN)Created   : $@$(NC)\n"

#------------------------------------------------------------------------------#
# specifications                                                               #
#------------------------------------------------------------------------------#

.PHONY: all \
		clean \
		fclean \
		re \
		lib \
		test \
		run \
		debug \
		valgrind
