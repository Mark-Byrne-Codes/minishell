/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_builtin.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbyrne <mbyrne@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 09:46:19 by mbyrne            #+#    #+#             */
/*   Updated: 2025/03/26 15:56:18 by mbyrne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * @brief Validates if a string is a valid environment variable identifier.
 * 
 * Checks if the name starts with a letter or underscore and contains only
 * alphanumeric characters and underscores thereafter.
 * 
 * @param name The identifier string to validate
 * @return int 1 if valid, 0 if invalid
 */
static int	is_valid_identifier(const char *name)
{
	if (!name || !*name)
		return (0);
	if (!ft_isalpha(*name) && *name != '_')
		return (0);
	name++;
	while (*name)
	{
		if (!ft_isalnum(*name) && *name != '_')
			return (0);
		name++;
	}
	return (1);
}

/**
 * @brief Extracts name and value from an export argument.
 * 
 * Splits the argument at the first '=' character into name and value parts.
 * Handles both cases where value is present (VAR=value) and absent (VAR).
 * 
 * @param arg The full argument string
 * @param name Pointer to store extracted name
 * @param value Pointer to store extracted value (NULL if no '=')
 * @return int 0 on success, 1 on memory allocation failure
 */
static int	extract_name_value(const char *arg, char **name, char **value)
{
	char	*equal_sign;
	char	*arg_copy;

	*name = NULL;
	*value = NULL;
	arg_copy = ft_strdup(arg);
	if (!arg_copy)
		return (1);
	equal_sign = ft_strchr(arg_copy, '=');
	if (equal_sign)
	{
		*equal_sign = '\0';
		*name = ft_strdup(arg_copy);
		*value = ft_strdup(equal_sign + 1);
	}
	else
		*name = ft_strdup(arg_copy);
	free(arg_copy);
	return (*name == NULL);
}

/**
 * @brief Internal function to set an environment variable.
 * 
 * Validates and processes a single export argument, updating the environment
 * if valid or printing an error message if invalid.
 * 
 * @param mini Pointer to minishell structure
 * @param arg The export argument to process
 * @return int 0 on success, 1 on validation failure
 */
static int	set_env_var_internal(t_mini *mini, const char *arg)
{
	char	*name;
	char	*value;
	int		ret;

	ret = extract_name_value(arg, &name, &value);
	if (ret || !is_valid_identifier(name))
	{
		if (name)
		{
			ft_putstr_fd("minishell: export: ", STDERR_FILENO);
			ft_putstr_fd(name, STDERR_FILENO);
			ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
			free(name);
		}
		free(value);
		return (1);
	}
	update_env_variable(mini, name, value);
	free(name);
	free(value);
	return (0);
}

/**
 * @brief Handles export arguments split across multiple tokens.
 * 
 * Processes cases where the value is in a separate argument (VAR= value)
 * by combining them into a single quoted string (VAR="value").
 * 
 * @param mini Pointer to minishell structure
 * @param args The argument array
 * @param i Pointer to current argument index (will be incremented)
 * @param ret Pointer to store return status
 * @return int SUCCESS (0) or ERROR (1) on memory allocation failure
 */
static int	handle_split_arg(t_mini *mini, char **args, int *i, int *ret)
{
	char	*full_arg;
	char	*temp;

	full_arg = ft_strjoin3(args[*i], "\"", args[*i + 1]);
	if (!full_arg)
		return (ERROR);
	temp = ft_strjoin(full_arg, "\"");
	free(full_arg);
	if (!temp)
		return (ERROR);
	if (set_env_var_internal(mini, temp) != 0)
		*ret = 1;
	free(temp);
	*i += 2;
	return (SUCCESS);
}

/**
 * @brief Implements the export builtin command.
 * 
 * Handles both cases:
 * 1. Without arguments - prints all environment variables in export format
 * 2. With arguments - processes each argument to add/update environment vars
 * 
 * @param mini Pointer to minishell structure
 * @param args Argument array (args[0] is "export")
 * @return int 0 on success, 1 if any argument was invalid, ERROR on fatal error
 */
int	export_builtin(t_mini *mini, char **args)
{
	int		i;
	int		ret;

	if (!args[1])
	{
		print_env_vars(mini->env);
		return (0);
	}
	i = 1;
	ret = 0;
	while (args[i])
	{
		if (args[i][ft_strlen(args[i]) - 1] == '=' && args[i + 1])
		{
			if (handle_split_arg(mini, args, &i, &ret) == ERROR)
				return (ERROR);
		}
		else
		{
			if (set_env_var_internal(mini, args[i]) != 0)
				ret = 1;
			i++;
		}
	}
	return (ret);
}
