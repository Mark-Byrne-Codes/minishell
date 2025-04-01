/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbyrne <mbyrne@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 09:17:23 by mbyrne            #+#    #+#             */
/*   Updated: 2025/04/01 10:51:06 by mbyrne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * @brief Checks if a given command is a builtin command.
 * 
 * This function checks if the provided command matches any of the known
 * builtin commands (echo, cd, pwd, export, unset, env, exit).
 * 
 * @param command The command string to check.
 * @return true if the command is a builtin, false otherwise.
 */
bool	is_builtin(char *command)
{
	if (command == NULL)
		return (false);
	if (ft_strcmp(command, "echo") == 0 || \
		ft_strcmp(command, "cd") == 0 || \
		ft_strcmp(command, "pwd") == 0 || \
		ft_strcmp(command, "export") == 0 || \
		ft_strcmp(command, "unset") == 0 || \
		ft_strcmp(command, "env") == 0 || \
		ft_strcmp(command, "exit") == 0)
	{
		return (true);
	}
	return (false);
}

/**
 * @brief Handles input/output redirection for builtin commands.
 * 
 * For single commands (not in a pipeline), this function saves the current
 * stdin/stdout and sets up any specified redirections.
 * 
 * @param mini Pointer to the main minishell structure.
 * @param cmd Pointer to the command structure.
 * @param saved_stdin Pointer to store the saved stdin file descriptor.
 * @param saved_stdout Pointer to store the saved stdout file descriptor.
 * @return int SUCCESS on success, ERROR on failure.
 */
static int	handle_io_for_builtin(t_mini *mini, t_command *cmd,
		int *saved_stdin, int *saved_stdout)
{
	*saved_stdin = dup(STDIN_FILENO);
	*saved_stdout = dup(STDOUT_FILENO);
	if (handle_redirection(mini, cmd) == ERROR)
	{
		restore_io(*saved_stdin, *saved_stdout);
		return (ERROR);
	}
	return (SUCCESS);
}

/**
 * @brief Executes simple builtin commands that don't modify the environment.
 * 
 * This function handles the execution of builtin commands that don't need to
 * modify the shell's environment (echo, pwd, env).
 * 
 * @param mini Pointer to the main minishell structure.
 * @param cmd Pointer to the command structure.
 * @return int The exit status of the executed command.
 */
static int	execute_simple_builtin(t_mini *mini, t_command *cmd)
{
	if (ft_strcmp(cmd->args[0], "echo") == 0)
		return (echo_builtin(cmd->args));
	else if (ft_strcmp(cmd->args[0], "pwd") == 0)
	{
		return (pwd_builtin());
	}
	else if (ft_strcmp(cmd->args[0], "env") == 0)
		return (env_builtin(mini));
	return (0);
}

/**
 * @brief Executes complex builtin commands that modify the environment.
 * 
 * This function handles the execution of builtin commands that modify the
 * shell's environment (cd, export, unset, exit).
 * 
 * @param mini Pointer to the main minishell structure.
 * @param cmd Pointer to the command structure.
 * @return int The exit status of the executed command.
 */
static int	execute_complex_builtin(t_mini *mini, t_command *cmd)
{
	if (ft_strcmp(cmd->args[0], "cd") == 0)
		return (cd_builtin(mini, cmd->args));
	else if (ft_strcmp(cmd->args[0], "export") == 0)
		return (export_builtin(mini, cmd->args));
	else if (ft_strcmp(cmd->args[0], "unset") == 0)
		return (unset_builtin(mini, cmd->args));
	else if (ft_strcmp(cmd->args[0], "exit") == 0)
		return (exit_builtin(mini, cmd->args));
	return (0);
}

/**
 * @brief Executes a builtin command.
 * 
 * This is the main function for executing builtin commands. It handles
 * redirections, executes the appropriate builtin function (either simple
 * or complex), and restores original file descriptors if needed.
 * 
 * @param mini Pointer to the main minishell structure.
 * @param cmd_idx Index of the command in the commands array.
 * @return int The exit status of the executed command, or ERROR on failure.
 */
int	execute_builtin(t_mini *mini, int cmd_idx)
{
	t_command	*cmd;
	int			saved_stdin;
	int			saved_stdout;
	int			result;

	cmd = &mini->commands[cmd_idx];
	if (handle_io_for_builtin(mini, cmd, &saved_stdin, &saved_stdout) == ERROR)
		return (ERROR);
	result = execute_simple_builtin(mini, cmd);
	mini->exit_status = result;
	if (result == 0)
		result = execute_complex_builtin(mini, cmd);
	restore_io(saved_stdin, saved_stdout);
	return (result);
}
