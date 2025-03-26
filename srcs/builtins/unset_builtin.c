/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_builtin.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbyrne <mbyrne@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 09:46:30 by mbyrne            #+#    #+#             */
/*   Updated: 2025/03/26 15:58:03 by mbyrne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * @brief Validates if a string is a valid environment variable identifier.
 * 
 * Checks that the identifier:
 * - Is not NULL or empty
 * - Doesn't start with a digit
 * - Contains only alphanumeric characters and underscores
 * 
 * @param str The identifier string to validate
 * @return int 1 if valid, 0 if invalid
 */
static int	is_valid_identifier(const char *str)
{
	if (!str || !*str || ft_isdigit(*str))
		return (0);
	while (*str)
	{
		if (!ft_isalnum(*str) && *str != '_')
			return (0);
		str++;
	}
	return (1);
}

/**
 * @brief Implements the unset builtin command.
 * 
 * Removes environment variables specified in arguments.
 * Validates each argument before removal and reports invalid identifiers.
 * 
 * @param mini Pointer to minishell structure containing environment
 * @param args Array of arguments (args[0] is "unset", 
 * args[1..n] are vars to unset)
 * @return int 0 if all variables were valid (even if not found), 
 *             1 if any invalid identifier was encountered
 * 
 * @note If no arguments are provided, prints "unset:" and returns 0
 * @note Error messages follow format: 
 *       "minishell: unset: `var': not a valid identifier"
 */
int	unset_builtin(t_mini *mini, char **args)
{
	int	status;
	int	i;

	status = 0;
	i = 1;
	if (!args[1])
		return (ft_putstr_fd("unset:\n", STDERR_FILENO), 0);
	while (args[i])
	{
		if (!is_valid_identifier(args[i]))
		{
			ft_putstr_fd("minishell: unset: `", STDERR_FILENO);
			ft_putstr_fd(args[i], STDERR_FILENO);
			ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
			status = 1;
		}
		else
		{
			remove_env_node(&mini->env, args[i]);
		}
		i++;
	}
	return (status);
}
