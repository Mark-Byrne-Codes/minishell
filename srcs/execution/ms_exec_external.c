/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_exec_external.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbyrne <mbyrne@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 14:55:49 by mbyrne            #+#    #+#             */
/*   Updated: 2025/03/23 15:01:15 by mbyrne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	handle_command_not_found(t_mini *mini, int cmd_idx)
{
	if (cmd_idx == 0)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(mini->commands[cmd_idx].args[0], 2);
		ft_putendl_fd(": command not found", 2);
	}
	mini->exit_status = 127;
	return (127);
}

static int	handle_execution_error(t_mini *mini, int cmd_idx)
{
	if (cmd_idx == 0)
	{
		ft_putstr_fd("minishell: ", 2);
		perror(mini->commands[cmd_idx].args[0]);
	}
	if (errno == ENOENT)
		exit(127);
	exit(126);
}

static void	close_other_pipes(t_mini *mini, int cmd_idx)
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

static int	execute_child_process(t_mini *mini, int cmd_idx, char *cmd_path)
{
	pid_t	pid;
	int		is_builtin;

	is_builtin = mini->commands[cmd_idx].is_builtin;
	pid = fork();
	if (pid == 0)
	{
		signal(SIGPIPE, SIG_IGN);
		if (is_builtin)
			exit(execute_builtin(mini, cmd_idx));
		close_other_pipes(mini, cmd_idx);
		execve(cmd_path, mini->commands[cmd_idx].args,
			env_list_to_array(mini->env));
		handle_execution_error(mini, cmd_idx);
	}
	else if (pid < 0)
		return (ERROR);
	mini->last_pid = pid;
	return (SUCCESS);
}

int	launch_external(t_mini *mini, int cmd_idx)
{
	char	*cmd_path;
	int		result;

	if (!mini->commands[cmd_idx].args || !mini->commands[cmd_idx].args[0])
		return (ERROR);
	if (mini->commands[cmd_idx].is_builtin)
		return (execute_child_process(mini, cmd_idx, NULL));
	cmd_path = get_command_path(mini->commands[cmd_idx].args[0], mini->env);
	if (!cmd_path)
		return (handle_command_not_found(mini, cmd_idx));
	if (mini->commands[cmd_idx].error)
	{
		free(cmd_path);
		mini->exit_status = 1;
		return (ERROR);
	}
	result = execute_child_process(mini, cmd_idx, cmd_path);
	free(cmd_path);
	mini->commands[cmd_idx].args = NULL;
	return (result);
}
