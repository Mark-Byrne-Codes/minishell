/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbyrne <mbyrne@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 09:17:23 by mbyrne            #+#    #+#             */
/*   Updated: 2025/03/23 12:21:40 by mbyrne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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

static int	handle_io_for_builtin(t_mini *mini, t_command *cmd,
		int *saved_stdin, int *saved_stdout)
{
	if (mini->num_commands == 1)
	{
		*saved_stdin = dup(STDIN_FILENO);
		*saved_stdout = dup(STDOUT_FILENO);
		if (handle_redirection(mini, cmd) == ERROR)
		{
			restore_io(*saved_stdin, *saved_stdout);
			return (ERROR);
		}
	}
	return (SUCCESS);
}

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
	if (result == 0)
		result = execute_complex_builtin(mini, cmd);
	if (mini->num_commands == 1)
		restore_io(saved_stdin, saved_stdout);
	return (result);
}
