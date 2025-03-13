# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mbyrne <mbyrne@student.hive.fi>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/03 15:33:31 by mbyrne            #+#    #+#              #
#    Updated: 2025/03/06 13:14:23 by mbyrne           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror -g
RL_FLAGS = -lreadline

GREEN   = \033[0;32m
YELLOW  = \033[0;33m
CYAN    = \033[0;36m
RED     = \033[0;31m
CLEAR   = \033[0m

OBJ_DIR =   ./objs/

LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

INCLUDES = -Iincludes -I$(LIBFT_DIR)/includes


SRCS_MAIN = srcs/main.c

SRCS_BUILTINS = srcs/builtins/ft_exit.c

SRCS_UTILS = srcs/utils/free.c \
				srcs/utils/helpers.c

SRCS_ENV = srcs/env/env_list.c \
				srcs/env/env_utils.c

SRCS_PARSE = srcs/parser/ms_lexer_utils.c \
					srcs/parser/ms_lexer.c



SRCS = $(SRCS_MAIN) $(SRCS_ENV) $(SRCS_UTILS) $(SRCS_PARSE)

OBJS = $(SRCS:srcs/%.c=$(OBJ_DIR)%.o)


all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(INCLUDES) $(OBJS) $(LIBFT) $(RL_FLAGS) -o $(NAME)

$(LIBFT):
	@$(MAKE) -C $(LIBFT_DIR)

$(OBJ_DIR)%.o: srcs/%.c
	@mkdir -p $(dir $@)
	@echo "$(YELLOW)Compiling: $<$(CLEAR)"
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	make clean -C $(LIBFT_DIR)
	rm -rf $(OBJ_DIR)

fclean: clean
	make fclean -C $(LIBFT_DIR)
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re