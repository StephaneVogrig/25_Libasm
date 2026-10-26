# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: svogrig < svogrig@studebt.42.fr>           +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/10/24 11:46:07 by svogrig           #+#    #+#              #
#    Updated: 2026/10/26 18:59:13 by svogrig          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

MAKEFLAGS 		+= --no-print-directory

BLINK_GREEN		:=	\033[5;32m
BGREEN			:=	\033[1;32m
BHYEL			:= 	\033[1;93m
BOLD			:=	\033[1m
NC				:=	\033[0m

# $(1) = titre de la bannière
define banner
	@banner_width=$$(printf '%s' "$(1)" | wc -m); \
	banner_tiret=$$(printf '━%.0s' $$(seq 1 $$banner_width)); \
	printf "\n"; \
	printf "\t\t$(BHYEL)┍━━%s━»•» 🌸 «•«━┑$(NC)\n" "$$banner_tiret"; \
	printf "\t\t$(BHYEL)  %s COMPILATION$(NC)\n" "$(1)"; \
	printf "\t\t$(BHYEL)┕━»•» 🌸 «•«━%s━━┙$(NC)\n" "$$banner_tiret"; \
	printf "\n"
endef

# $(1) = cible à construire
define build
	$(call banner,$(1))
	@$(MAKE) -j $(1)
	@printf "\n"
	@printf "\t$(BGREEN)$(1) : COMPILATION FINISHED !$(NC)\n"
endef

LIB_NAME 		:= libasm.a
TEST_NAME		:= test_libasm

# includes --------------------------------------------------------------------#

DIR_INC				:=	include

I_FLAG				:=	$(addprefix -I,$(DIR_INC)) -MMD -MP

# sources ---------------------------------------------------------------------#

DIR_SRC			:=	src

SRC_ASM			:= ft_strlen.s

SRC_C			:= main.c

# objects ---------------------------------------------------------------------#

OBJ_DIR			:=	.build
OBJ_ASM			:= $(addprefix $(OBJ_DIR)/, $(SRC_ASM:.s=.o))
OBJ_C			:= $(addprefix $(OBJ_DIR)/, $(SRC_C:.c=.o))

# compilation -----------------------------------------------------------------#

NASM			:= nasm
NASM_FLAGS 		:= -f elf64

AR				:=	ar
ARFLAGS			:=	-r -c -s

CC				:=	cc
CC_FLAGS		:=	-Wall -Wextra -Werror

-include $(OBJ_C:.o=.d)

#------------------------------------------------------------------------------#
# rules                                                                        #
#------------------------------------------------------------------------------#

all: test

clean:
	@rm -rf $(OBJ_DIR) && printf "Cleaning : $(OBJ_DIR)\n"

fclean: clean
	@rm -f $(LIB_NAME) && printf "Cleaning : $(LIB_NAME)\n"
	@rm -f $(TEST_NAME) && printf "Cleaning : $(TEST_NAME)\n"

re: fclean all

libasm: $(LIB_NAME)
# 	@if ! $(MAKE) -q $(LIB_NAME); then \
# 		$(call build,$(LIB_NAME)) \
# 	fi

test: libasm $(TEST_NAME)
# 	@if ! $(MAKE) -q $(TEST_NAME); then \
# 		$(call build,$(TEST_NAME)) \
# 	fi

#------------------------------------------------------------------------------#
# compilation                                                                  #
#------------------------------------------------------------------------------#

$(OBJ_DIR)/%.o: $(DIR_SRC)/%.s
	@mkdir -p $(@D)
	@printf "Compiling : %s\n" $@
	@$(NASM) $(NASM_FLAGS) $< -o $@

$(OBJ_DIR)/%.o: $(DIR_SRC)/%.c
	@mkdir -p $(@D)
	@printf "Compiling : %s\n" $@
	@$(CC) $(CC_FLAGS) $(I_FLAG) -c $< -o $@

#------------------------------------------------------------------------------#
# linkage                                                                      #
#------------------------------------------------------------------------------#

$(LIB_NAME): $(OBJ_ASM)
	@$(AR) $(ARFLAGS) $(LIB_NAME) $(OBJ_ASM) && printf "Created : %s\n" $@

$(TEST_NAME): $(OBJ_C) $(LIB_NAME)
	@$(CC) $(CC_FLAGS) $^ -o $@ && printf "Created : %s\n" $@

#------------------------------------------------------------------------------#
# specifications                                                               #
#------------------------------------------------------------------------------#

.PHONY: all \
		clean \
		fclean \
		re \
		libasm \
		test
