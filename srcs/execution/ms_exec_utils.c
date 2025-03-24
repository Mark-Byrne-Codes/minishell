/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_exec_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbyrne <mbyrne@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 14:55:17 by mbyrne            #+#    #+#             */
/*   Updated: 2025/03/24 09:38:54 by mbyrne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	add_argument(t_command *cmd, char *arg, int arg_idx)
{
	char	**new_args;
	int		i;

	new_args = malloc(sizeof(char *) * (arg_idx + 2));
	if (!new_args)
		return (ERROR);
	i = 0;
	while (i < arg_idx)
	{
		new_args[i] = cmd->args[i];
		i++;
	}
	new_args[arg_idx] = ft_strdup(arg);
	if (!new_args[arg_idx])
	{
		free(new_args);
		return (ERROR);
	}
	new_args[arg_idx + 1] = NULL;
	free(cmd->args);
	cmd->args = new_args;
	return (SUCCESS);
}

static char *check_local_or_absolute_path(char *cmd)
{
    struct stat file_stat;

    if (access(cmd, F_OK) == 0)
    {
        if (stat(cmd, &file_stat) == 0 && S_ISREG(file_stat.st_mode))
        {
            if (access(cmd, X_OK) == 0)
                return (ft_strdup(cmd));
            errno = EACCES;
            return (ft_strdup(cmd));
        }
    }
    return (NULL);
}

static char *search_path_directories(char *cmd, t_env *env)
{
    char *path;
    char **dirs;
    int i;

    path = get_env_value(env, "PATH");
    if (!path)
        return (NULL);
    dirs = ft_split(path, ':');
    if (!dirs)
        return (NULL);
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

char *get_command_path(char *cmd, t_env *env)
{
    char *result;

    if (ft_strchr(cmd, '/'))
        return (check_local_or_absolute_path(cmd));
    result = check_local_or_absolute_path(cmd);
    if (result)
        return (result);
    return (search_path_directories(cmd, env));
}

int	wait_for_children(t_mini *mini, int last_status)
{
	int		status;
	int		last_exit_status;
	pid_t	pid;

	last_exit_status = 0;
	pid = waitpid(-1, &status, WUNTRACED);
	while (pid > 0)
	{
		if (pid == mini->last_pid)
		{
			if (WIFEXITED(status))
			{
				mini->exit_status = WEXITSTATUS(status);
				last_exit_status = mini->exit_status;
			}
			else if (WIFSIGNALED(status))
				mini->exit_status = 128 + WTERMSIG(status);
		}
		pid = waitpid(-1, &status, WUNTRACED);
	}
	if (mini->num_commands > 1)
		return (mini->exit_status);
	if (last_status != ERROR)
		return (last_exit_status);
	return (mini->exit_status = last_status);
}


