/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_exec_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbyrne <mbyrne@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 14:55:17 by mbyrne            #+#    #+#             */
/*   Updated: 2025/03/24 13:20:25 by mbyrne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*check_direct_cmd(char *cmd)
{
	struct stat	file_stat;

	if (access(cmd, F_OK) == 0 && !ft_strchr(cmd, '/'))
	{
		if (stat(cmd, &file_stat) == 0 && S_ISREG(file_stat.st_mode))
		{
			if (access(cmd, X_OK) != 0)
			{
				errno = EACCES;
				return (ft_strdup(cmd));
			}
		}
	}
	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, F_OK) == 0)
			return (cmd);
		return (NULL);
	}
	return (NULL);
}

char	*get_command_path(char *cmd, t_env *env)
{
	char	*path;
	char	**dirs;
	int		i;
	char	*result;

	result = check_direct_cmd(cmd);
	if (result)
		return (result);
	path = get_env_value(env, "PATH");
	dirs = ft_split(path, ':');
	i = -1;
	while (dirs[++i])
	{
		path = ft_strjoin3(dirs[i], "/", cmd);
		if (access(path, X_OK) == 0)
		{
			free_string_array(dirs);
			return (path);
		}
		free(path);
	}
	free_string_array(dirs);
	return (NULL);
}

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
