/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_builtin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbyrne <mbyrne@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 09:17:19 by mbyrne            #+#    #+#             */
/*   Updated: 2025/04/01 09:15:54 by mbyrne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * @brief Prints an error message for cd command failures.
 * 
 * Formats and prints a standardized error message when the cd command fails,
 * including the path that caused the error and the system error message.
 * 
 * @param path The path that caused the cd command to fail.
 */
static void	print_cd_error(char *path)
{
	ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
	ft_putstr_fd(path, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putstr_fd(strerror(errno), STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
}

/**
 * @brief Safely duplicates a string, handling NULL input.
 * 
 * Wrapper around ft_strdup that safely handles NULL input by returning NULL
 * instead of causing a segmentation fault.
 * 
 * @param str The string to duplicate.
 * @return char* A newly allocated duplicate of the string, or NULL.
 */
static char	*safe_strdup(const char *str)
{
	if (str)
		return (ft_strdup(str));
	return (NULL);
}

/**
 * @brief Updates or creates an environment variable.
 * 
 * Modifies an existing environment variable or creates a new one if it doesn't
 * exist. Handles memory allocation and deallocation appropriately.
 * 
 * @param mini Pointer to the main minishell structure.
 * @param var_name The name of the environment variable to update/create.
 * @param new_value The new value to set for the variable (can be NULL).
 */
void	update_env_variable(t_mini *mini, char *var_name, char *new_value)
{
	t_env	*env_node;
	t_env	*new_node;
	char	*value_copy;

	env_node = find_env_node(mini->env, var_name);
	value_copy = safe_strdup(new_value);
	if (env_node)
	{
		if (new_value)
		{
			free(env_node->value);
			env_node->value = value_copy;
		}
		else
			free(value_copy);
	}
	else
	{
		new_node = malloc(sizeof(t_env));
		new_node->name = ft_strdup(var_name);
		new_node->value = value_copy;
		new_node->next = NULL;
		add_env_node(&mini->env, new_node);
	}
}

/**
 * @brief Changes the current working directory and updates environment vars.
 * 
 * Handles the actual directory change operation, updates PWD and OLDPWD
 * environment variables, and prints errors if the operation fails.
 * 
 * @param mini Pointer to the main minishell structure.
 * @param path The path to change directory to.
 * @return int 0 on success, 1 on failure.
 */
static int	change_directory(t_mini *mini, char *path)
{
	char	oldpwd[PATH_MAX];
	char	newpwd[PATH_MAX];

	if (!getcwd(oldpwd, sizeof(oldpwd)))
		return (1);
	if (chdir(path) != 0)
	{
		print_cd_error(path);
		return (1);
	}
	update_env_variable(mini, "OLDPWD", oldpwd);
	if (!getcwd(newpwd, sizeof(newpwd)))
		return (1);
	update_env_variable(mini, "PWD", newpwd);
	return (0);
}

/**
 * @brief The main cd builtin command implementation.
 * 
 * Handles the cd command arguments, determines the target directory
 * (defaulting to HOME if no argument is provided), and executes the
 * directory change.
 * 
 * @param mini Pointer to the main minishell structure.
 * @param args The command arguments (args[0] is "cd", args[1] is path).
 * @return int 0 on success, 1 on failure.
 */
int	cd_builtin(t_mini *mini, char **args)
{
	char	*path;
	int		arg_count;

	arg_count = 0;
	while (args[arg_count])
		arg_count++;
	if (arg_count > 2)
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", STDERR_FILENO);
		return (1);
	}
	if (arg_count == 1 || (arg_count == 2 && ft_strcmp(args[1], "~") == 0))
	{
		path = getenv("HOME");
		if (!path)
		{
			ft_putstr_fd("minishell: cd: HOME not set\n", STDERR_FILENO);
			return (1);
		}
	}
	else
		path = args[1];
	return (change_directory(mini, path));
}
