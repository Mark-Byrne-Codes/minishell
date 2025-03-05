/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbyrne <mbyrne@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 15:33:52 by mbyrne            #+#    #+#             */
/*   Updated: 2025/03/05 18:01:01 by mbyrne           ###   ########.fr       */
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
	TOKEN_HEREDOC
}	t_token_type;

/* Represents a single token in the parsed input.  This is part of a
 * dynamically allocated *array* of tokens (managed within the t_mini struct).
 * type: The type of the token (see e_token_type).
 * str: The string value of the token (dynamically allocated).
 * expanded: Flag: 1 if expanded, 0 if not.
 */

typedef struct s_token
{
	t_token_type	type;
	char			*str;
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
 * in_builtin: Flag for builtins.
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
	int			in_builtin;
}	t_mini;



#endif
