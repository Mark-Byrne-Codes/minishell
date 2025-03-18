/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbyrne <mbyrne@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 15:33:52 by mbyrne            #+#    #+#             */
/*   Updated: 2025/03/18 18:08:21 by elehtone         ###   ########.fr       */
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
# include "../libft/libft.h"

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
	char		**argv;
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
t_env	*find_env_node(t_env *head, char *name);
void	free_env_list(t_env *head);
void	free_string_array(char **array);
char	*join_and_free(char *s1, char *s2);
void	clean_exit(t_mini *mini);
void    free_token(t_token *token);

//ms_lexer.c - Superman's worst enemy
int	parse_line(t_lexer *lexer, char *command);

//ms_lexer_tokens.c - functions to handle assigning tokens
void	*fun_variable_string(t_lexer *lexer, char *command);
void	*fun_word_string(t_lexer *lexer, char *command);
void	*fun_double_quote_string(t_lexer *lexer, char *command);
void	*fun_single_quote_string(t_lexer *lexer, char *command);

//ms_lexer_utils.c - helpers for the lexer, list handling
void	fun_lex_struct_init(t_lexer *lexer);
void	*fun_add_entry(t_lexer *lexer, char *str, int len, t_token_type token);
void	fun_flag_flipper(int *flag);
int	fun_check_ifs(unsigned int c);
void	unfun_add_entry_fail(t_lexer *lexer, int end);

#endif
