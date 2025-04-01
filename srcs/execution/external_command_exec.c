/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_command_exec.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbyrne <mbyrne@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 14:55:49 by mbyrne            #+#    #+#             */
/*   Updated: 2025/04/01 08:44:56 by mbyrne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * @brief Handles the case when a command is not found
 * 
 * @param mini Pointer to the main minishell structure
 * @param cmd_idx Index of the command that wasn't found
 * @return int Returns 127 (standard "command not found" exit status)
 * 
 * @note Prints appropriate error messages depending on whether the command
 * contained a path separator or not.
 */
static int	handle_command_not_found(t_mini *mini, int cmd_idx)
{
	if (cmd_idx == 0 || mini->num_commands == 1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(mini->commands[cmd_idx].args[0], 2);
		if (ft_strchr(mini->commands[cmd_idx].args[0], '/'))
			ft_putendl_fd(": No such file or directory", 2);
		else
			ft_putendl_fd(": command not found", 2);
	}
	mini->exit_status = 127;
	return (127);
}

/**
 * @brief Handles errors that occur during command execution
 * 
 * @param mini Pointer to the main minishell structure
 * @param cmd_idx Index of the command that failed
 * @return int Returns appropriate exit status (126 or 127)
 * 
 * @note Special case for directories returns 126 if trying to execute a dir.
 * Distinguishes between "not found" (127) and other errors (126).
 */
static int	handle_execution_error(t_mini *mini, int cmd_idx)
{
	struct stat	file_stat;
	char		*cmd;

	cmd = mini->commands[cmd_idx].args[0];
	if (cmd_idx == 0)
	{
		if (stat(cmd, &file_stat) == 0 && S_ISDIR(file_stat.st_mode))
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(cmd, 2);
			ft_putendl_fd(": Is a directory", 2);
			mini->exit_status = 126;
			return (126);
		}
		else
		{
			ft_putstr_fd("minishell: ", 2);
			perror(cmd);
		}
	}
	if (errno == ENOENT)
		mini->exit_status = 127;
	else
		mini->exit_status = 126;
	return (mini->exit_status);
}

/**
 * @brief Closes all pipes that aren't needed by the current command
 * 
 * @param mini Pointer to the main minishell structure
 * @param cmd_idx Index of the current command
 * 
 * @note This prevents pipe leaks in child processes and ensures proper
 * cleanup of file descriptors.
 */
void	close_other_pipes(t_mini *mini, int cmd_idx)
{
	int	i;

	i = 0;
	while (i < mini->num_commands)
	{
		if (i != cmd_idx && (mini->commands[i].pipe_read != -1
				|| mini->commands[i].pipe_write != -1))
		{
			if (mini->commands[i].pipe_read != -1)
				close(mini->commands[i].pipe_read);
			if (mini->commands[i].pipe_write != -1)
				close(mini->commands[i].pipe_write);
		}
		i++;
	}
}

/**
 * @brief Executes a command in a child process
 * 
 * @param mini Pointer to the main minishell structure
 * @param cmd_idx Index of the command to execute
 * @param cmd_path Full path to the command executable
 * @return int Returns SUCCESS on successful fork, ERROR on failure
 * 
 * @note Handles both builtin and external commands. For external commands,
 * it sets up the environment array and executes via execve.
 */
static int	execute_child_process(t_mini *mini, int cmd_idx, char *cmd_path)
{
	pid_t	pid;
	char	**args;
	int		exit_status;
	char	**env_array;

	args = mini->commands[cmd_idx].args;
	pid = fork();
	if (pid == 0)
	{
		close_other_pipes(mini, cmd_idx);
		env_array = env_list_to_array(mini->env);
		execve(cmd_path, args, env_array);
		free_string_array(env_array);
		exit_status = handle_execution_error(mini, cmd_idx);
		exit(exit_status);
	}
	else if (pid < 0)
		return (ERROR);
	mini->last_pid = pid;
	mini->commands[cmd_idx].pid = pid;
	return (SUCCESS);
}

/**
 * @brief Launches an external command or builtin
 * 
 * @param mini Pointer to the main minishell structure
 * @param cmd_idx Index of the command to launch
 * @return int Returns SUCCESS on success, ERROR on failure
 * 
 * @note Handles command path resolution, memory management for command paths,
 * and error cases before execution.
 */
int	launch_external(t_mini *mini, int cmd_idx)
{
	char	*cmd_path;
	int		result;
	int		is_allocated;

	if (!mini->commands[cmd_idx].args || !mini->commands[cmd_idx].args[0])
		return (SUCCESS);
	cmd_path = get_command_path(mini->commands[cmd_idx].args[0], mini->env);
	is_allocated = (cmd_path != mini->commands[cmd_idx].args[0]);
	if (!cmd_path)
		return (handle_command_not_found(mini, cmd_idx));
	if (mini->commands[cmd_idx].error)
	{
		if (is_allocated)
			free(cmd_path);
		mini->exit_status = 1;
		return (ERROR);
	}
	result = execute_child_process(mini, cmd_idx, cmd_path);
	if (is_allocated)
		free(cmd_path);
	return (result);
}
