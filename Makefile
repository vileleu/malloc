# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vileleu <vileleu@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/07/17 13:36:31 by vileleu           #+#    #+#              #
#    Updated: 2026/06/18 17:30:10 by vileleu          ###   ########.fr        #
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

SRCS		= malloc.c realloc.c free.c show_alloc_mem.c \
			block/block.c \
			zone/zone.c \
			heap/heap.c \
			utils/utils.c \
			error/error.c

INCS		= -I$(DIR_INCS)
OBJS 		= $(patsubst %.c,$(DIR_OBJS)/%.o,$(SRCS))
DEPS 		= $(patsubst $(DIR_OBJS)/%.o,$(DIR_DEPS)/%.d,$(OBJS))

NAME 		= libft_malloc_$(HOSTTYPE).so
LINK	 	= libft_malloc.so
CC			= gcc
CFLAGS		= -Wall -Wextra -Werror -pthread
OFLAGS		= -fPIC -MMD -MP -MF $(patsubst $(DIR_OBJS)/%.o,$(DIR_DEPS)/%.d,$@)
RM			= rm -rf

all:		 $(NAME)

$(DIR_OBJS)/%.o: $(DIR_SRCS)/%.c
			@mkdir -p $(dir $@) $(patsubst $(DIR_OBJS)/%,$(DIR_DEPS)/%,$(dir $@))
			@printf "$(BLUE)$< -> $(ORANGE)$@ $(BLUE)-> $(ORANGE)$(patsubst $(DIR_OBJS)/%.o,$(DIR_DEPS)/%.d,$@)\n$(RESET)"
			@$(CC) $(CFLAGS) $(OFLAGS) $(INCS) -c $< -o $@

$(NAME):	$(OBJS)
			@printf "\n$(BLUE)Compiling $(NAME) ... $(RESET)"
			@$(CC) -shared -pthread -fPIC $(OBJS) -o $(NAME)
			@ln -sf $(NAME) $(LINK)
			@printf "$(GREEN)[✔]\n[$(NAME) done]$(RESET)\n"

-include	$(DEPS)

test:
			@$(CC) $(CFLAGS) test.c $(INCS) -L. -lft_malloc -o test && \
			LD_LIBRARY_PATH=. ./test

clean:
			@printf "$(BLUE)Clean objects and dependencies ...$(RESET)"
			@$(RM) $(DIR_OBJS) $(DIR_DEPS)
			@printf "$(GREEN) [✔]$(RESET)\n"

fclean:		clean
			@printf "$(BLUE)Clean $(NAME) ...$(RESET)"
			@$(RM) $(NAME) $(LINK) test
			@printf "$(GREEN) [✔]$(RESET)\n"

re:			fclean all

.PHONY:		all clean fclean re test