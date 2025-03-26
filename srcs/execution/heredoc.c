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

static int	handle_heredoc_input(t_command *cmd, char *delim, int pipe_fd[2])
{
	char	*line;

	while (1)
	{
		write(STDOUT_FILENO, "> ", 2);
		line = readline("");
		if (!line) 
		{
			write(STDOUT_FILENO, "\n", 1); 
			return (SUCCESS);
		}
		if (ft_strcmp(line, delim) == 0)
		{
			free(line);
			return (SUCCESS);
		}
		if (process_heredoc_line(cmd, line, pipe_fd) == ERROR)
			return (ERROR);
	}
}

static void	handle_child_process(t_command *cmd, char *delim, int pipe_fd[2])
{
	setup_heredoc_signals();
	close(pipe_fd[0]);
	if (handle_heredoc_input(cmd, delim, pipe_fd) == ERROR)
	{
		close(pipe_fd[1]);
		free(delim);
		exit(EXIT_FAILURE);
	}
	close(pipe_fd[1]);
	free(delim);
	exit(EXIT_SUCCESS);
}

static int	handle_parent_process(t_command *cmd, int pipe_fd[2], pid_t pid)
{
	int	status;

	close(pipe_fd[1]);
	waitpid(pid, &status, 0);
	if ((WIFSIGNALED(status) && WTERMSIG(status) == SIGINT) || 
		(WIFEXITED(status) && WEXITSTATUS(status) == 130))
	{
		close(pipe_fd[0]);
		cmd->error = 1;
		g_exit_status = 130;
		signal(SIGINT, signal_handler_interactive);
		signal(SIGQUIT, SIG_IGN);
		return (ERROR);
	}
	cmd->fd_in = pipe_fd[0];
	cmd->has_input_redir = 1;
	cmd->is_heredoc = 1;
	signal(SIGINT, signal_handler_interactive);
	signal(SIGQUIT, SIG_IGN);
	return (SUCCESS);
}

int	setup_heredoc_delim(t_command *cmd, char *delim)
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
	if (!unq_delim || (pid = fork()) < 0)
	{
		if (unq_delim)
			free(unq_delim);
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		return (ERROR);
	}
	if (pid == 0)
		handle_child_process(cmd, unq_delim, pipe_fd);
	free(unq_delim);
	return (handle_parent_process(cmd, pipe_fd, pid));
}
