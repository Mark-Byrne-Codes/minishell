# ================================= COLORS =================================== #
BOLD	= \033[1m
RED		= \033[0;31m
GREEN	= \033[0;32m
YELLOW	= \033[0;33m
BLUE	= \033[0;34m
PURPLE	= \033[0;35m
CYAN	= \033[0;36m
RESET	= \033[0m

# ================================ SETTINGS ================================== #
NAME		= minishell
CC			= cc
CFLAGS		= -Wall -Wextra -Werror
RL_FLAGS	= -lreadline -L/usr/local/opt/readline/lib
OBJ_DIR		= ./objs/
LIBFT_DIR	= libft
LIBFT		= $(LIBFT_DIR)/libft.a
INCLUDES	= -Iincludes -I$(LIBFT_DIR)/includes

# ================================ SOURCES =================================== #
SRCS_MAIN	= srcs/main/main.c \
			  srcs/main/prompt.c \
			  srcs/main/signals.c \
			  srcs/main/init.c

SRCS_BUILTINS = srcs/builtins/exit_builtin.c \
				srcs/builtins/echo_builtin.c \
				srcs/builtins/pwd_builtin.c \
				srcs/builtins/env_builtin.c \
				srcs/builtins/cd_builtin.c \
				srcs/builtins/export_builtin.c \
				srcs/builtins/unset_builtin.c \
				srcs/builtins/builtin_utils.c

SRCS_UTILS	= srcs/utils/free.c \
			  srcs/utils/helpers.c \
			  srcs/utils/error.c

SRCS_ENV	= srcs/env/env.c \
			  srcs/env/env_utils.c

SRCS_EXEC	= srcs/execution/command_exec.c \
			  srcs/execution/redirection.c \
			  srcs/execution/pipes.c \
			  srcs/execution/process_control.c \
			  srcs/execution/path.c \
			  srcs/execution/external_command_exec.c

SRCS_PARSE	= srcs/parser/lexer_utils.c \
			  srcs/parser/lexer.c \
			  srcs/parser/token_types.c \
			  srcs/parser/token_expansion.c \
			  srcs/parser/variable_expansion.c \
			  srcs/parser/command_parser.c \
			  srcs/parser/token_processor.c \
			  srcs/parser/heredoc.c

SRCS		= $(SRCS_MAIN) $(SRCS_ENV) $(SRCS_UTILS) $(SRCS_BUILTINS) $(SRCS_EXEC) $(SRCS_PARSE)
OBJS		= $(SRCS:srcs/%.c=$(OBJ_DIR)%.o)

# ================================ RULES ===================================== #
all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	@echo "$(CYAN)$(BOLD)🔗 Linking $(NAME)...$(RESET)"
	@$(CC) $(CFLAGS) $(INCLUDES) $(OBJS) $(LIBFT) $(RL_FLAGS) -o $(NAME)
	@echo "$(GREEN)$(BOLD)✅ Successfully built $(NAME)!$(RESET)"

$(LIBFT):
	@echo "$(YELLOW)$(BOLD)🛠️  Building libft...$(RESET)"
	@$(MAKE) -C $(LIBFT_DIR)
	@echo "$(GREEN)$(BOLD)✔️  libft ready!$(RESET)"

$(OBJ_DIR)%.o: srcs/%.c
	@mkdir -p $(dir $@)
	@echo "$(BLUE)🔧 Compiling: $(PURPLE)$<$(RESET)"
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	@echo "$(RED)🧹 Cleaning object files...$(RESET)"
	@$(MAKE) -C $(LIBFT_DIR) clean
	@rm -rf $(OBJ_DIR)
	@echo "$(GREEN)✔️  Object files removed!$(RESET)"

fclean: clean
	@echo "$(RED)🔥 Full clean...$(RESET)"
	@$(MAKE) -C $(LIBFT_DIR) fclean
	@rm -f $(NAME)
	@echo "$(GREEN)✔️  Everything cleaned!$(RESET)"

re: fclean all

.PHONY: all clean fclean re