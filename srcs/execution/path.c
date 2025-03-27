/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbyrne <mbyrne@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 13:00:40 by mbyrne            #+#    #+#             */
/*   Updated: 2025/03/27 11:44:18 by mbyrne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * @brief Checks if a command can be executed directly (without PATH lookup).
 * 
 * Handles two cases:
 * 1. Commands without slashes - checks if they exist in current directory
 * 2. Commands with slashes - treats as direct path and checks existence
 * 
 * @param cmd The command string to check
 * @return char* Duplicate of cmd if valid (must be freed), NULL otherwise
 * 
 * @note Sets errno to EACCES if file exists but isn't executable
 * @note Returns the original cmd string (not duplicated) for paths with slashes
 */
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

/**
 * @brief Finds the full path to an executable command.
 * 
 * Searches for the command in this order:
 * 1. Checks if command can be executed directly (via check_direct_cmd)
 * 2. Searches through PATH environment variable directories
 * 
 * @param cmd The command name to find
 * @param env The environment variables list
 * @return char* Full path to executable (must be freed), or NULL if not found
 * 
 * @note Allocates new memory for the returned path string
 * @note Handles PATH environment variable lookup if needed
 * @note Returns NULL if command isn't found in any PATH directory
 */
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
