# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vileleu <vileleu@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/09/20 18:11:41 by vileleu           #+#    #+#              #
#    Updated: 2026/06/14 23:43:38 by vileleu          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

BLUE = 		\033[0;34m
GREEN = 	\033[0;32m
LIGHTBLUE = \033[1;34m
RED = 		\033[0;31m
YELLOW = 	\033[1;33m
ORANGE = 	\033[0;33m
MAGENTA = 	\033[0;35m
RESET = 	\033[0m

DIR_SRCS	= srcs
DIR_INCS	= includes
DIR_OBJS	= objects
DIR_DEPS 	= dependencies

SRCS		= 	ft_atoi.c ft_bzero.c \
				ft_isalnum.c ft_isalpha.c \
				ft_isascii.c ft_isdigit.c ft_isprint.c \
				ft_memccpy.c ft_memchr.c \
				ft_memcmp.c ft_memcpy.c ft_memmove.c \
				ft_memset.c ft_power.c \
				ft_putchar_fd.c ft_putendl_fd.c \
				ft_putnbr_fd.c ft_putstr_fd.c \
				ft_strchr.c ft_strcmp.c \
				ft_strlcat.c ft_strlcpy.c ft_strlen.c \
				ft_strncmp.c ft_strnstr.c \
				ft_strrchr.c \
				ft_tolower.c ft_toupper.c \

INCS		= -I $(DIR_INCS)
OBJS 		= $(patsubst %.c,$(DIR_OBJS)/%.o,$(SRCS))
DEPS 		= $(patsubst $(DIR_OBJS)/%.o,$(DIR_DEPS)/%.d,$(OBJS))

NAME		= libft
CC			= gcc
CFLAGS		= -Wall -Wextra -Werror
OFLAGS		= -MMD -MP -MF $(patsubst $(DIR_OBJS)/%.o,$(DIR_DEPS)/%.d,$@)
RM			= rm -rf

$(DIR_OBJS)/%.o: $(DIR_SRCS)/%.c
			@mkdir -p $(dir $@) $(patsubst $(DIR_OBJS)/%,$(DIR_DEPS)/%,$(dir $@))
			@printf "$(BLUE)\b->$(RESET)"
			@$(CC) $(CFLAGS) $(OFLAGS) $(INCS) -c $< -o $@

$(NAME):	$(OBJS)
			@printf "\n$(BLUE)Compiling libft ... $(RESET)"
			@ar rc $(NAME).a $(OBJS)
			@printf "$(GREEN)[$(NAME) done][✔]$(RESET)\n"

-include	$(DEPS)

all:		$(NAME)

clean:
			@printf "$(BLUE)Clean objects and dependencies ...$(RESET)"
			@$(RM) $(DIR_OBJS) $(DIR_DEPS)
			@printf "$(GREEN) [✔]$(RESET)\n"

fclean:
			@printf "$(BLUE)Fclean objects and dependencies ...$(RESET)"
			@$(RM) $(DIR_OBJS) $(DIR_DEPS)
			@printf "$(GREEN) [✔]$(RESET)\n"
			@printf "$(BLUE)Fclean executable file ...$(RESET)"
			@$(RM) $(NAME).a
			@printf "$(GREEN) [✔]$(RESET)\n"

re:			fclean all

.PHONY:		all clean fclean re