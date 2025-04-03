/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbyrne <mbyrne@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 15:30:12 by mbyrne            #+#    #+#             */
/*   Updated: 2025/03/30 15:28:05 by mbyrne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * @brief Processes a single line of heredoc input
 * 
 * Expands variables in the line and writes it to the heredoc pipe.
 * Handles all necessary memory management for the input line.
 * 
 * @param cmd Command structure containing execution context
 * @param mini Pointer to the main minishell structure
 * @param line Input line to process (will be freed)
 * @param pipe_fd Pipe file descriptors [read, write]
 * @return int SUCCESS (0) on success, ERROR (1) on failure
 * 
 * @note Closes both pipe ends and returns ERROR if expansion fails
 * @warning Frees the input line regardless of success/failure
 */
static int	process_heredoc_line(t_mini *mini, char *line, int pipe_fd[2])
{
	char	*expanded_line;

	expanded_line = expand_variables(mini, line, 0);
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
 * @brief Handles the heredoc input loop
 * 
 * Reads input line by line until delimiter is found or error occurs.
 * 
 * @param cmd Command structure
 * @param mini Pointer to the main minishell structure
 * @param delim The heredoc delimiter (unquoted)
 * @param pipe_fd Pipe file descriptors
 * @return int SUCCESS if delimiter found, ERROR on failure
 * 
 * @note Displays "> " prompt for each line
 * @note Returns SUCCESS on EOF (Ctrl+D) with empty line
 * @note Properly handles SIGINT interruption via global g_signal
 */
static int	handle_heredoc_input(t_mini *mini, char *delim, int pipe_fd[2])
{
	char	*line;

	while (1)
	{
		write(STDOUT_FILENO, "> ", 2);
		line = readline("");
		if (!line)
		{
			ft_putstr_fd("minishell: warning: heredoc delimited by EOF\n", 2);
			return (SUCCESS);
		}
		if (ft_strcmp(line, delim) == 0)
		{
			free(line);
			return (SUCCESS);
		}
		if (process_heredoc_line(mini, line, pipe_fd) == ERROR)
			return (ERROR);
	}
}

/**
 * @brief Child process execution for heredoc
 * 
 * Sets up signal handlers and processes heredoc input in a child process.
 * 
 * @param cmd Command structure
 * @param mini Pointer to the main minishell structure
 * @param delim Heredoc delimiter
 * @param pipe_fd Pipe file descriptors
 * 
 * @note Exits with EXIT_FAILURE (1) on error
 * @note Exits with EXIT_SUCCESS (0) on normal completion
 * @warning This function does not return - it exits the child process
 */
static void	handle_child_process(t_mini *mini, char *delim, int pipe_fd[2])
{
	setup_heredoc_signals();
	close(pipe_fd[0]);
	if (handle_heredoc_input(mini, delim, pipe_fd) == ERROR)
	{
		close(pipe_fd[1]);
		clean_exit(mini);
		exit(EXIT_FAILURE);
	}
	close(pipe_fd[1]);
	free(delim);
	clean_exit(mini);
	exit(EXIT_SUCCESS);
}

/**
 * @brief Parent process handling for heredoc
 * 
 * Waits for child completion and handles the result.
 * 
 * @param cmd Command structure
 * @param mini Pointer to the main minishell structure
 * @param pipe_fd Pipe file descriptors
 * @param pid Child process ID
 * @return int SUCCESS if heredoc completed, ERROR if interrupted/failed
 * 
 * @note Restores default signal handlers after completion
 * @note Sets command's input redirection fields on success
 * @note Sets g_signal to 130 (SIGINT) if interrupted
 */
static int	handle_parent_process(t_command *cmd, int pipe_fd[2], pid_t pid)
{
	int	status;

	close(pipe_fd[1]);
	waitpid(pid, &status, 0);
	if ((WIFSIGNALED(status) && WTERMSIG(status) == SIGINT) || \
		(WIFEXITED(status) && WEXITSTATUS(status) == 130))
	{
		close(pipe_fd[0]);
		cmd->error = 1;
		g_signal = 130;
		signal(SIGINT, signal_handler_interactive);
		signal(SIGQUIT, SIG_IGN);
		return (ERROR);
	}
	if (cmd->has_input_redir && cmd->fd_in != STDIN_FILENO)
		close(cmd->fd_in);
	cmd->fd_in = pipe_fd[0];
	cmd->has_input_redir = 1;
	cmd->is_heredoc = 1;
	signal(SIGINT, signal_handler_interactive);
	signal(SIGQUIT, SIG_IGN);
	return (SUCCESS);
}

/**
 * @brief Main heredoc setup function
 * 
 * Creates pipe, forks process, and manages heredoc execution.
 * 
 * @param cmd Command structure
 * @param mini Pointer to the main minishell structure
 * @param delim Heredoc delimiter (may be quoted)
 * @return int SUCCESS if heredoc ready, ERROR on failure
 * 
 * @note Removes quotes from delimiter before use
 * @note Handles all pipe and process cleanup
 * @note Manages both parent and child process execution paths
 */
int	setup_heredoc_delim(t_command *cmd, t_mini *mini, char *delim)
{
	int		pipe_fd[2];
	char	*unq_delim;
	pid_t	pid;

	if (pipe(pipe_fd) == -1)
	{
		cmd->error = 1;
		return (ERROR);
	}
	unq_delim = remove_quotes(delim);
	pid = fork();
	if (!unq_delim || pid < 0)
	{
		if (unq_delim)
			free(unq_delim);
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		return (ERROR);
	}
	if (pid == 0)
		handle_child_process(mini, unq_delim, pipe_fd);
	free(unq_delim);
	return (handle_parent_process(cmd, pipe_fd, pid));
}
