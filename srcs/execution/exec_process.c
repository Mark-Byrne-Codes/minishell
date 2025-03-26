/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_process.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbyrne <mbyrne@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 14:55:17 by mbyrne            #+#    #+#             */
/*   Updated: 2025/03/26 12:05:53 by mbyrne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * @brief Executes a single command from the command array
 * 
 * @param mini Pointer to the main minishell structure
 * @param i Index of the command to execute
 * @return int Returns the execution status
 * 
 * @note Handles both builtin and external commands:
 * - Builtins execute directly when they're the only command
 * - All other cases (external commands or piped builtins) fork a child process
 */
int	execute_single_command(t_mini *mini, int i)
{
	int	status;

	if (!mini->commands[i].args || !mini->commands[i].args[0])
		return (SUCCESS);
	if (mini->commands[i].is_builtin && mini->num_commands == 1)
		status = execute_builtin(mini, i);
	else
		status = launch_external(mini, i);
	return (status);
}

/**
 * @brief Initializes the exit status based on command context
 * 
 * @param mini Pointer to the main minishell structure
 * @param last_cmd_error Flag indicating if last command had an error
 * @return int Initial exit status value
 * 
 * @note Special handling for pipelines vs single commands:
 * - For pipelines, defaults to 0 unless last command had an error
 * - For single commands, uses the existing exit status if set
 */
static int	init_exit_status(t_mini *mini, int last_cmd_error)
{
	if (mini->num_commands > 1)
	{
		if (last_cmd_error)
			return (1);
		return (0);
	}
	if (mini->exit_status != 0)
		return (mini->exit_status);
	return (0);
}

/**
 * @brief Processes the status of a completed child process
 * 
 * @param mini Pointer to the main minishell structure
 * @param pid Process ID of the completed child
 * @param status Wait status of the child process
 * @param last_exit_status Current last exit status
 * @return int Updated last exit status
 * 
 * @note Handles both normal exits (WIFEXITED) and signals (WIFSIGNALED)
 * Only updates the main exit status for the last process in a pipeline
 */
static int	process_child_status(t_mini *mini, pid_t pid, int status,
	int last_exit_status)
{
	if (pid == mini->last_pid)
	{
		if (WIFEXITED(status))
		{
			mini->exit_status = WEXITSTATUS(status);
			if (!(mini->num_commands > 1
					&& mini->commands[mini->num_commands - 1].error))
				last_exit_status = mini->exit_status;
		}
		else if (WIFSIGNALED(status))
			mini->exit_status = 128 + WTERMSIG(status);
	}
	return (last_exit_status);
}

/**
 * @brief Waits for all child processes to complete and collects their status
 * 
 * @param mini Pointer to the main minishell structure
 * @param last_status Status of the last executed command
 * @return int Final exit status for the command sequence
 * 
 * @note Handles process termination in three scenarios:
 * 1. Normal command execution (WIFEXITED)
 * 2. Signal termination (WIFSIGNALED)
 * 3. Command errors (syntax, redirection, etc.)
 * Manages exit status differently for pipelines vs single commands
 */
int	wait_for_children(t_mini *mini, int last_status)
{
	int		status;
	int		last_exit_status;
	pid_t	pid;
	int		last_cmd_error;

	last_cmd_error = mini->commands[mini->num_commands - 1].error;
	last_exit_status = init_exit_status(mini, last_cmd_error);
	pid = waitpid(-1, &status, WUNTRACED);
	while (pid > 0)
	{
		last_exit_status = process_child_status(mini, pid, status,
				last_exit_status);
		pid = waitpid(-1, &status, WUNTRACED);
	}
	if (mini->num_commands > 1)
		mini->exit_status = last_exit_status;
	else if (mini->exit_status == 0 && last_status != SUCCESS)
		mini->exit_status = last_status;
	return (mini->exit_status);
}
