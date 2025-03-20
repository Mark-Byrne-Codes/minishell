/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbyrne <mbyrne@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 15:33:52 by mbyrne            #+#    #+#             */
/*   Updated: 2025/03/18 09:06:23 by mbyrne           ###   ########.fr       */
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
# include <stdio.h>
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

enum 
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

/* Represents a single token in the parsed input.  This is part of a
 * dynamically allocated *array* of tokens (managed within the t_mini struct).
 * type: The type of the token (see e_token_type).
 * str: The string value of the token (dynamically allocated).
 * expanded: Flag: 1 if expanded, 0 if not.
 */

 typedef struct s_token
 {
	 t_token_type	token;
	 char			*string;
	 int				len;
	 int				expanded;
 }	t_token;

typedef struct s_redirection
{
	int             type;
	char            *file;
	struct s_redirection *next;
} t_redirection;

/* Represents a single command to be executed. This includes the command,
 * its arguments, and any redirection information.  These form a
 * dynamically allocated *array* of commands (managed within t_mini).
 * argv: Argument vector (NULL-terminated array of strings).
 * fd_in: Input file descriptor (0 = stdin, -1 = error).
 * fd_out: Output file descriptor (1 = stdout, -1 = error).
 * pipe_read: File descriptor for the read end of a pipe
 * pipe_write: File descriptor for the write end of a pipe
*/
typedef struct s_command
{
	char		**args;
	int		fd_in;
	int		fd_out;
	int		pipe_read;
	int		pipe_write;
	int		is_builtin;
	int		exit_status;
	int		has_input_redir;
	int		has_output_redir;
	int		append;
	int		is_heredoc;
	struct s_mini	*mini;
	int		redir_type;
	char	*redir_file;
	int		error;
	t_redirection   *redirections;
}	t_command;

/* Represents a single environment variable (NAME=value).  These form a
 * *singly-linked list*.  The linked list is managed within the t_mini struct.
 * name:  Name of the environment variable (e.g., "HOME").
 * value: Value of the environment variable (e.g., "/home/user").
 * next:  Pointer to the next environment variable.
 */
typedef struct s_env
{
	char			*name;
	char			*value;
	struct s_env	*next;
}	t_env;

/* Represents the overall state of the Minishell.  This is the main
 * data structure that's passed around between functions.
 * tokens: Pointer to the array of tokens.
 * num_tokens: Number of tokens currently stored.
 * token_capacity: Allocated size of the tokens array.
 * commands: Pointer to the array of commands.
 * num_commands: Number of commands currently stored.
 * command_capacity: Allocated size of the commands array.
 * env: Pointer to the head of the env list.
 * envp: The raw environment.
 * fd_in:  Standard input for the shell.
 * fd_out: Standard output for the shell.
 * last_pid: PID of the last forked child process.
 * exit_status: Exit status of the last command.
 * should_exit: Flag to signal exit (e.g., from 'exit' builtin).
 * is_builtin: Flag for builtins.
 */
typedef struct s_mini
{
	t_token		*tokens;
	int			num_tokens;
	int			token_capacity;
	t_command	*commands;
	int			num_commands;
	int			command_capacity;
	t_env		*env;
	char		**envp;
	int			fd_in;
	int			fd_out;
	int			pipe_read;
	int			pipe_write;
	pid_t		last_pid;
	int			exit_status;
	int			should_exit;
	int			is_builtin;
}	t_mini;

void	remove_env_node(t_env **head, char *name);
char	**env_list_to_array(t_env *head);
t_env	*copy_env(char **envp);
t_env	*create_env_node(char *name, char *value);
void	add_env_node(t_env **head, t_env *new_node);
t_env	*find_env_node(t_env *head, const char *name);
void	free_env_list(t_env *head);
void	free_string_array(char **array);
char	*join_and_free(char *s1, char *s2);
void	clean_exit(t_mini *mini);
int		cd_builtin(t_mini *mini, char **args);
int		env_builtin(t_mini *mini);
int		echo_builtin(char **args);
int		exit_builtin(t_mini *mini, char **args);
int		pwd_builtin(void);
int		export_builtin(t_mini *mini, char **args);
int 	unset_builtin(t_mini *mini, char **args);
size_t	ft_strspn(const char *s1, const char *s2);
char	*ft_strndup(const char *s, size_t n);
void	update_env_variable(t_mini *mini, char *var_name, char *new_value);
void	setup_signal_handlers(t_mini *mini);
char *ft_strcat(char *dest, const char *src);
char *ft_strcpy(char *dest, const char *src);
char *ft_str_join(char *s1, char *s2);
char *get_prompt(void);
int execute_builtin(t_mini *mini, int cmd_idx);
bool is_builtin(char *command);
int setup_pipes(t_mini *mini, int cmd_idx);
void connect_pipes(t_mini *mini, int cmd_idx);
void close_pipes(t_mini *mini, int cmd_idx);
int execute_pipeline(t_mini *mini);
void exec_command(t_mini *mini, int cmd_idx);
int handle_redirection(t_mini *mini, t_command *cmd);
int handle_single_redir(t_command *cmd, t_token *file_token);
int handle_heredoc(t_command *cmd, t_token *delim_token);
int handle_output_redir(t_command *cmd, t_token *file_token, int append);
void restore_io(int saved_stdin, int saved_stdout);
int execute_commands(t_mini *mini);
int launch_external(t_mini *mini, int cmd_idx);
int wait_for_children(t_mini *mini, int last_status);
char *get_command_path(char *cmd, t_env *env);
char *get_env_value(t_env *env, char *key);
void exit_error(char *msg, int code);
char *ft_strjoin3(char const *s1, char const *s2, char const *s3);
void free_commands(t_mini *mini);
void free_tokens(t_mini *mini);
int	init_commands(t_mini *mini, int num_commands);
int	add_argument(t_command *cmd, char *arg, int arg_idx);
void free_redirections(t_command *cmd);
char	*remove_quotes(const char *str);
int		setup_heredoc_delim(t_command *cmd, char *delim);
int		setup_input_redir_file(t_command *cmd, char *file);
int		setup_output_redir_file(t_command *cmd, char *file, int append);
int		apply_redirections(t_command *cmd);
char	*remove_quotes(const char *str);
void	file_error(t_mini *mini, t_command *cmd, char *file);
void	print_error(t_mini *mini, char *cmd, char *msg, int status);
int	execute_single_command(t_mini *mini, int i);


//ms_lexer.c - stuff to lex, count special characters and so on
int	parse_line(t_lexer *lexer, char *command, t_mini *mini);
void	*fun_variable_string(t_lexer *lexer, char *cmd);
void	*fun_word_string(t_lexer *lexer, char *cmd);
void	*fun_double_quote_string(t_lexer *lexer, char *command);
void	*fun_single_quote_string(t_lexer *lexer, char *command);

//ms_lexer_utilities.c - stuff to lex, count special characters and so on
void	*fun_add_entry(t_lexer *lexer, char *str, int len, t_token_type token);
void	fun_lex_struct_init(t_lexer *lexer);
void	fun_flag_flipper(int *flag);
int	fun_check_ifs(unsigned int c);
void    free_token(t_token *token);
void	unfun_add_entry_fail(t_lexer *lexer, int end);


char    *expand_token(t_mini *mini, t_token *token);
char    *expand_variables(t_mini *mini, char *str, int in_quotes);
char    *expand_tilde(t_mini *mini, char *str);
char    *ft_strjoinchar(const char *s1, char c);

int	ms_handle_pipe(t_mini *mini, t_list **temp, int *cmd_idx, int *arg_idx);
int	ms_handle_token_expansion(t_mini *mini, t_command *cmd, 
		t_token *token, int *arg_idx);
int	ms_handle_redirection(t_mini *mini, t_command *cmd, t_list **temp);
int	ms_process_tokens(t_mini *mini, t_lexer *lexer);

#endif