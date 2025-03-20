/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipes.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbyrne <mbyrne@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 11:16:37 by mbyrne            #+#    #+#             */
/*   Updated: 2025/03/18 10:57:47 by mbyrne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	setup_pipes(t_mini *mini, int cmd_idx)
{
	int	pipe_fd[2];

	if (cmd_idx < mini->num_commands - 1)
	{
		if (pipe(pipe_fd) == -1)
			return (ERROR);
		mini->commands[cmd_idx].pipe_write = pipe_fd[1];
		mini->commands[cmd_idx + 1].pipe_read = pipe_fd[0];
	}
	else
		mini->commands[cmd_idx].pipe_write = -1;
	return (SUCCESS);
}

void	connect_pipes(t_mini *mini, int cmd_idx)
{
	t_command	*cmd;

	cmd = &mini->commands[cmd_idx];
	if (cmd->pipe_read != -1)
	{
		dup2(cmd->pipe_read, STDIN_FILENO);
		close(cmd->pipe_read);
	}
	if (cmd->pipe_write != -1)
	{
		dup2(cmd->pipe_write, STDOUT_FILENO);
		close(cmd->pipe_write);
	}
}

void	close_pipes(t_mini *mini, int cmd_idx)
{
	t_command	*cmd;

	cmd = &mini->commands[cmd_idx];
	if (cmd->pipe_read != STDIN_FILENO)
		close(cmd->pipe_read);
	if (cmd->pipe_write != STDOUT_FILENO)
		close(cmd->pipe_write);
}

int	execute_pipeline(t_mini *mini)
{
	pid_t	pid;
	int		i;

	i = -1;
	while (++i < mini->num_commands)
	{
		if (setup_pipes(mini, i) == ERROR)
			return (ERROR);
		pid = fork();
		if (pid == 0)
		{
			connect_pipes(mini, i);
			handle_redirection(mini, &mini->commands[i]);
			exec_command(mini, i);
		}
		else if (pid < 0)
			return (ERROR);
		close_pipes(mini, i);
	}
	return (wait_for_children(mini, SUCCESS));
}

void	exec_command(t_mini *mini, int cmd_idx)
{
	char	*path;

	if (mini->commands[cmd_idx].is_builtin)
		exit(execute_builtin(mini, cmd_idx));
	path = get_command_path(mini->commands[cmd_idx].args[0], mini->env);
	if (!path)
		exit_error("command not found", 127);
	execve(path, mini->commands[cmd_idx].args, env_list_to_array(mini->env));
	exit_error("execve failed", 126);
}