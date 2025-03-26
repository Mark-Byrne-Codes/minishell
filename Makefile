
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


SRCS_MAIN = srcs/main/main.c \
			srcs/main/prompt.c \
			srcs/main/signals.c \
			srcs/main/init.c \

SRCS_BUILTINS = srcs/builtins/exit_builtin.c \
				srcs/builtins/echo_builtin.c \
				srcs/builtins/pwd_builtin.c \
				srcs/builtins/env_builtin.c \
				srcs/builtins/cd_builtin.c \
				srcs/builtins/export_builtin.c \
				srcs/builtins/unset_builtin.c \
				srcs/builtins/builtin_utils.c
				

SRCS_UTILS = srcs/utils/free.c \
				srcs/utils/helpers.c \
				srcs/utils/error.c \

SRCS_ENV = srcs/env/env.c \
				srcs/env/env_utils.c

SRCS_EXEC = 	srcs/execution/exec_cmds.c \
				srcs/execution/exec_redirects.c \
				srcs/execution/exec_pipes.c \
				srcs/execution/exec_process.c \
				srcs/execution/exec_path.c \
				srcs/execution/exec_external.c \
				srcs/execution/exec_heredoc.c
 
SRCS_PARSE = srcs/parser/ms_lexer_utils.c \
					srcs/parser/ms_lexer.c \
					srcs/parser/ms_lexer_tokens.c \
					srcs/parser/ms_expand.c \
					srcs/parser/ms_expand_var.c \
					srcs/parser/ms_parser.c \
					srcs/parser/ms_token_processor.c \


SRCS = $(SRCS_MAIN) $(SRCS_ENV) $(SRCS_UTILS) $(SRCS_BUILTINS) $(SRCS_EXEC) $(SRCS_PARSE)
 
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


