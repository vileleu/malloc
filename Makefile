# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vileleu <vileleu@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/07/17 13:36:31 by vileleu           #+#    #+#              #
#    Updated: 2026/06/15 16:46:10 by vileleu          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

BLUE		= \033[0;34m
GREEN		= \033[0;32m
BLUELIGHT	= \033[1;34m
RED			= \033[0;31m
YELLOW		= \033[1;33m
ORANGE		= \033[0;33m
MAGENTA		= \033[0;35m
RESET		= \033[0m

DIR_SRCS		= srcs
DIR_INCS		= includes
DIR_OBJS		= objects
DIR_DEPS 		= dependencies
DIR_LIBS		= libraries
NAME_LIBFT		= libft
DIR_LIBFT		= $(DIR_LIBS)/$(NAME_LIBFT)
DIR_INCS_LIBFT	= $(DIR_LIBS)/$(NAME_LIBFT)/$(DIR_INCS)
LIBFT			= $(DIR_LIBFT)/$(NAME_LIBFT).a

SRCS		= malloc.c realloc.c free.c show_alloc_mem.c \
			block/block.c \
			zone/zone.c \
			heap/heap.c \
			error/error.c

INCS		= -I$(DIR_INCS) -I$(DIR_INCS_LIBFT)
OBJS 		= $(patsubst %.c,$(DIR_OBJS)/%.o,$(SRCS))
DEPS 		= $(patsubst $(DIR_OBJS)/%.o,$(DIR_DEPS)/%.d,$(OBJS))

NAME 		= libmalloc_$(HOSTTYPE).so
LINK	 	= libmalloc.so
CC			= gcc
CFLAGS		= -Wall -Wextra -Werror -lpthread
OFLAGS		= -fPIC -MMD -MP -MF $(patsubst $(DIR_OBJS)/%.o,$(DIR_DEPS)/%.d,$@)
RM			= rm -rf

all:		 $(NAME)

$(LIBFT):
			@printf "$(BLUE)Compiling $(LIBFT) ... $(RESET)\n"
			@$(MAKE) -C $(DIR_LIBFT)

$(DIR_OBJS)/%.o: $(DIR_SRCS)/%.c | $(LIBFT)
			@mkdir -p $(dir $@) $(patsubst $(DIR_OBJS)/%,$(DIR_DEPS)/%,$(dir $@))
			@printf "$(BLUE)$< -> $(ORANGE)$@ $(BLUE)-> $(ORANGE)$(patsubst $(DIR_OBJS)/%.o,$(DIR_DEPS)/%.d,$@)\n$(RESET)"
			@$(CC) $(CFLAGS) $(OFLAGS) $(INCS) -c $< -o $@

$(NAME):	$(OBJS)
			@printf "\n$(BLUE)Compiling $(NAME) ... $(RESET)"
			@$(CC) -shared $(OBJS) $(LIBFT) -o $(NAME)
			@ln -sf $(NAME) $(LINK)
			@printf "$(GREEN)[✔]\n[$(NAME) done]$(RESET)\n"

-include	$(DEPS)

test:
			@$(CC) main.c $(INCS) -L. -lmalloc -o test && LD_LIBRARY_PATH=. ./test
			@$(RM) test

install:
			@sudo apt update
			@sudo apt upgrade

clean:
			@printf "$(BLUE)Clean libraries ...$(RESET)\n"
			@$(MAKE) -C $(DIR_LIBFT) clean
			@printf "$(BLUE)Clean objects and dependencies ...$(RESET)"
			@$(RM) $(DIR_OBJS) $(DIR_DEPS)
			@printf "$(GREEN) [✔]$(RESET)\n"

fclean:
			@printf "$(BLUE)Fclean libraries ...$(RESET)\n"
			@$(MAKE) -C $(DIR_LIBFT) fclean
			@printf "$(BLUE)Fclean objects and dependencies ...$(RESET)"
			@$(RM) $(DIR_OBJS) $(DIR_DEPS)
			@printf "$(GREEN) [✔]$(RESET)\n"
			@printf "$(BLUE)Fclean $(NAME) ...$(RESET)"
			@$(RM) $(NAME) $(LINK) test
			@printf "$(GREEN) [✔]$(RESET)\n"

re:			fclean all

.PHONY:		all clean fclean re