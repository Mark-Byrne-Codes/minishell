/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_exec.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbyrne <mbyrne@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 11:16:34 by mbyrne            #+#    #+#             */
/*   Updated: 2025/03/27 11:44:12 by mbyrne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * Saves the current standard input and output file descriptors
 * 
 * param saved_stdin Pointer to store the saved stdin fd
 * param saved_stdout Pointer to store the saved stdout fd
 * return int Returns SUCCESS on success, ERROR on failure
 */
static int	setup_io(int *saved_stdin, int *saved_stdout)
{
	*saved_stdin = dup(STDIN_FILENO);
	*saved_stdout = dup(STDOUT_FILENO);
	if (*saved_stdin == -1 || *saved_stdout == -1)
		return (ERROR);
	return (SUCCESS);
}

/**
 *Sets up pipe redirections for the current command
 * 
 * param mini Pointer to the main minishell structure
 * param i Index of the current command
 * return int Returns SUCCESS on success, ERROR on failure
 */
static int	handle_pipe_setup(t_mini *mini, int i)
{
	if (i < mini->num_commands - 1)
	{
		if (mini->commands[i].pipe_write != -1 && \
			dup2(mini->commands[i].pipe_write, STDOUT_FILENO) == -1)
			return (ERROR);
	}
	if (i > 0)
	{
		if (mini->commands[i].pipe_read != -1 && \
			dup2(mini->commands[i].pipe_read, STDIN_FILENO) == -1)
			return (ERROR);
	}
	return (SUCCESS);
}

/**
 * Closes all file descriptors associated with a command
 * 
 * param cmd Pointer to the command structure
 */
static void	close_fds(t_command *cmd)
{
	if (cmd->pipe_write != -1)
	{
		close(cmd->pipe_write);
		cmd->pipe_write = -1;
	}
	if (cmd->pipe_read != -1)
	{
		close(cmd->pipe_read);
		cmd->pipe_read = -1;
	}
	if (cmd->fd_in != STDIN_FILENO)
	{
		close(cmd->fd_in);
		cmd->fd_in = STDIN_FILENO;
	}
	if (cmd->fd_out != STDOUT_FILENO)
	{
		close(cmd->fd_out);
		cmd->fd_out = STDOUT_FILENO;
	}
}

/**
 * Prepares a command for execution by setting up I/O and pipes
 * 
 * param mini Pointer to the main minishell structure
 * param i Index of the current command
 * param saved_stdin Pointer to store the saved stdin fd
 * param saved_stdout Pointer to store the saved stdout fd
 * return int Returns SUCCESS on success, ERROR on failure
 */
static int	prepare_command(t_mini *mini, int i,
			int *saved_stdin, int *saved_stdout)
{
	if (setup_io(saved_stdin, saved_stdout) == ERROR)
		return (ERROR);
	if (setup_pipes(mini, i) == ERROR)
		return (ERROR);
	if (handle_pipe_setup(mini, i) == ERROR)
		return (ERROR);
	if (mini->commands[i].redirections)
	{
		if (handle_redirection(mini, &mini->commands[i]) == ERROR)
		{
			mini->commands[i].error = 1;
			return (ERROR);
		}
	}
	return (SUCCESS);
}

/**
 * Executes all commands in the pipeline
 * 
 * param mini Pointer to the main minishell structure
 * return int Returns the exit status of the last command executed
 */
int	execute_commands(t_mini *mini)
{
	int	i;
	int	status;
	int	pipe_failed;
	int	saved_stdin;
	int	saved_stdout;

	i = 0;
	pipe_failed = 0;
	while (i < mini->num_commands)
	{
		if (prepare_command(mini, i, &saved_stdin, &saved_stdout) == ERROR)
		{
			pipe_failed = 1;
			mini->commands[i].error = 1;
		}
		else
			status = execute_single_command(mini, i);
		close_fds(&mini->commands[i]);
		restore_io(saved_stdin, saved_stdout);
		i++;
	}
	if (mini->num_commands == 1 && mini->commands[0].error)
		mini->exit_status = 1;
	return (wait_for_children(mini, pipe_failed * 1 + !pipe_failed * status));
}
