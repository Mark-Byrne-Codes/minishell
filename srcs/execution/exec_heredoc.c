/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbyrne <mbyrne@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 15:30:12 by mbyrne            #+#    #+#             */
/*   Updated: 2025/03/26 12:06:37 by mbyrne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * @brief Processes a single line of heredoc input, expanding variables
 * 
 * @param cmd Pointer to the command structure
 * @param line The input line to process
 * @param pipe_fd Pipe file descriptors for heredoc communication
 * @return int Returns SUCCESS on success, ERROR on failure
 * 
 * @note Expands vars in the line, writes to pipe, and handles memory cleanup.
 * Closes pipe and returns ERROR if expansion fails.
 */
static int	process_heredoc_line(t_command *cmd, char *line, int pipe_fd[2])
{
	char	*expanded_line;

	expanded_line = expand_variables(cmd->mini, line, 0);
	free(line);
	if (!expanded_line)
	{
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		return (ERROR);
	}
	write(pipe_fd[1], expanded_line, ft_strlen(expanded_line));
	write(pipe_fd[1], "\n", 1);
	free(expanded_line);
	return (SUCCESS);
}

/**
 * @brief Handles the heredoc input loop until delimiter is found
 * 
 * @param cmd Pointer to the command structure
 * @param delim The heredoc delimiter (unquoted)
 * @param pipe_fd Pipe file descriptors for heredoc communication
 * @return int Returns SUCCESS on success, ERROR on failure
 * 
 * @note Reads input line by line until delimiter is found. Processes each line
 * and handles proper cleanup on EOF or error. Frees the delimiter when done.
 */
static int	handle_heredoc_input(t_command *cmd, char *delim, int pipe_fd[2])
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			close(pipe_fd[0]);
			close(pipe_fd[1]);
			free(delim);
			return (ERROR);
		}
		if (ft_strcmp(line, delim) == 0)
		{
			free(line);
			free(delim);
			break ;
		}
		if (process_heredoc_line(cmd, line, pipe_fd) == ERROR)
		{
			free(delim);
			return (ERROR);
		}
	}
	return (SUCCESS);
}

/**
 * @brief Sets up a heredoc redirection for a command
 * 
 * @param cmd Pointer to the command structure
 * @param delim The heredoc delimiter (may be quoted)
 * @return int Returns SUCCESS on success, ERROR on failure
 * 
 * @note Creates a pipe, removes quotes from delimiter, processes input,
 * and sets up the command's input redirection. Marks command as having heredoc.
 * Handles all necessary cleanup in error cases.
 */
int	setup_heredoc_delim(t_command *cmd, char *delim)
{
	int		pipe_fd[2];
	char	*unquoted_delimiter;

	if (pipe(pipe_fd) == -1)
	{
		cmd->error = 1;
		return (ERROR);
	}
	unquoted_delimiter = remove_quotes(delim);
	if (!unquoted_delimiter)
	{
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		return (ERROR);
	}
	if (handle_heredoc_input(cmd, unquoted_delimiter, pipe_fd) == ERROR)
		return (ERROR);
	close(pipe_fd[1]);
	cmd->fd_in = pipe_fd[0];
	cmd->has_input_redir = 1;
	cmd->is_heredoc = 1;
	return (SUCCESS);
}
