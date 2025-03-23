/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbyrne <mbyrne@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 15:14:33 by mbyrne            #+#    #+#             */
/*   Updated: 2025/03/23 15:15:44 by mbyrne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <string.h>
# include <sys/stat.h>
# include <errno.h>
# include <linux/limits.h>
# include "../libft/libft.h"
# include <stdbool.h>

typedef enum e_token_type
{
	TOKEN_EMPTY,
	TOKEN_WORD,
	TOKEN_VARIABLE,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_REDIR_APPEND,
	TOKEN_HEREDOC,
	TOKEN_SINGLE_Q_STRING,
	TOKEN_DOUBLE_Q,
	TOKEN_DOUBLE_Q_CONTENT,
	TOKEN_IFS
}	t_token_type;

enum e_status
{
	SUCCESS = 0,
	ERROR = 1
};

typedef struct s_lexer
{
	t_list	*dictionary;
	int		squote;
	int		dquote;
	int		pipes;
	int		vars;
	int		red_in;
	int		red_delim;
	int		red_out;
	int		red_append;
}	t_lexer;

typedef struct s_token
{
	t_token_type	token;
	char			*string;
	int				len;
	int				expanded;
}	t_token;

typedef struct s_redirection
{
	int						type;
	char					*file;
	struct s_redirection	*next;
}	t_redirection;

typedef struct s_command
{
	char				**args;
	int					fd_in;
	int					fd_out;
	int					pipe_read;
	int					pipe_write;
	int					is_builtin;
	int					exit_status;
	int					has_input_redir;
	int					has_output_redir;
	int					append;
	int					is_heredoc;
	struct s_mini		*mini;
	int					error;
	t_redirection		*redirections;
}	t_command;

typedef struct s_env
{
	char			*name;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_mini
{
	t_token			*tokens;
	int				num_tokens;
	int				token_capacity;
	t_command		*commands;
	int				num_commands;
	int				command_capacity;
	t_env			*env;
	char			**envp;
	int				fd_in;
	int				fd_out;
	int				pipe_read;
	int				pipe_write;
	pid_t			last_pid;
	int				exit_status;
	int				should_exit;
	int				is_builtin;
}	t_mini;

// Environment Functions
t_env	*copy_env(char **envp);
t_env	*create_env_node(char *name, char *value);
void	add_env_node(t_env **head, t_env *new_node);
t_env	*find_env_node(t_env *head, const char *name);
void	remove_env_node(t_env **head, char *name);
void	update_env_variable(t_mini *mini, char *var_name, char *new_value);
char	**env_list_to_array(t_env *head);
void	free_env_list(t_env *head);
char	*get_env_value(t_env *env, char *key);

// Memory Management & String Handling
void	free_string_array(char **array);
char	*join_and_free(char *s1, char *s2);
char	*ft_strjoin3(char const *s1, char const *s2, char const *s3);
char	*remove_quotes(const char *str);
char	*get_var_name(const char *str);

// Built-in Commands
int		cd_builtin(t_mini *mini, char **args);
int		env_builtin(t_mini *mini);
int		echo_builtin(char **args);
int		exit_builtin(t_mini *mini, char **args);
int		pwd_builtin(void);
int		export_builtin(t_mini *mini, char **args);
int		unset_builtin(t_mini *mini, char **args);
bool	is_builtin(char *command);
int		execute_builtin(t_mini *mini, int cmd_idx);

// Command Execution & Pipelines
int		execute_commands(t_mini *mini);
int		execute_pipeline(t_mini *mini);
void	exec_command(t_mini *mini, int cmd_idx);
int		launch_external(t_mini *mini, int cmd_idx);
int		wait_for_children(t_mini *mini, int last_status);
char	*get_command_path(char *cmd, t_env *env);
int		execute_single_command(t_mini *mini, int i);

// Redirection Handling
int		handle_redirection(t_mini *mini, t_command *cmd);
void	restore_io(int saved_stdin, int saved_stdout);
int		setup_heredoc_delim(t_command *cmd, char *delim);
int		setup_input_redir_file(t_command *cmd, char *file);
int		setup_output_redir_file(t_command *cmd, char *file, int append);
int		apply_redirections(t_command *cmd);
void	free_redirections(t_command *cmd);
int		add_redirection(t_command *cmd, t_redirection *new_redir);
t_redirection	*create_redirection(t_token *type, t_token *file);

// Pipe Handling
int		setup_pipes(t_mini *mini, int cmd_idx);
void	connect_pipes(t_mini *mini, int cmd_idx);
void	close_pipes(t_mini *mini, int cmd_idx);

// Command & Token Management
int		init_commands(t_mini *mini, int num_commands);
int		add_argument(t_command *cmd, char *arg, int arg_idx);
void	free_commands(t_mini *mini);
void	free_tokens(t_mini *mini);

// Error Handling & Signal Handling
void	exit_error(char *msg, int code);
void	file_error(t_mini *mini, t_command *cmd, char *file);
void	print_error(t_mini *mini, char *cmd, char *msg, int status);
void	clean_exit(t_mini *mini);
void	setup_signal_handlers(t_mini *mini);

// Prompt & Variable Expansion
char	*get_prompt(void);
char	*expand_exit_status(t_mini *mini);
char	*expand_env_var(t_mini *mini, const char *var_name);

// Lexer Functions
void	*fun_charwise(t_lexer *lexer, char *command);
int		parse_line(t_lexer *lexer, char *command, t_mini *mini);
void	*fun_variable_string(t_lexer *lexer, char *cmd);
void	*fun_word_string(t_lexer *lexer, char *cmd);
void	*fun_double_quote_string(t_lexer *lexer, char *command);
void	*fun_single_quote_string(t_lexer *lexer, char *command);
void	fun_lex_struct_init(t_lexer *lexer);
void	*fun_add_entry(t_lexer *lexer, char *str, int len, t_token_type token);
void	fun_flag_flipper(int *flag);
int		fun_check_ifs(unsigned int c);
void	unfun_add_entry_fail(t_lexer *lexer, int end);
int		fun_check_any_quote(unsigned int c);

// Token Expansion
char	*expand_token(t_mini *mini, t_token *token);
char	*expand_variables(t_mini *mini, char *str, int in_quotes);
char	*expand_tilde(t_mini *mini, char *str);
void	free_token(t_token *token);

// Token Handling
int		ms_handle_pipe(t_mini *mini, t_list **temp, int *cmd_idx, int *arg_idx);
int		ms_handle_token_expansion(t_mini *mini, t_command *cmd,
			t_token *token, int *arg_idx);
int		ms_handle_redirection(t_mini *mini, t_command *cmd, t_list **temp);
int		ms_process_tokens(t_mini *mini, t_lexer *lexer);

#endif
