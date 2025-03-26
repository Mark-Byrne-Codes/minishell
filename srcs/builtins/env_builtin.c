/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_builtin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbyrne <mbyrne@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 09:47:16 by mbyrne            #+#    #+#             */
/*   Updated: 2025/03/26 15:57:14 by mbyrne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * @brief Implements the env builtin command.
 * 
 * Prints all environment variables in the format "NAME=value" for each variable
 * that has a non-empty value. Variables with empty values are skipped.
 * 
 * @param mini Pointer to the main minishell structure containing the environment
 * @return int Always returns 0 (success)
 */
int	env_builtin(t_mini *mini)
{
	t_env	*current;

	current = mini->env;
	while (current)
	{
		if (current->value && current->value[0] != '\0')
		{
			ft_putstr_fd(current->name, STDOUT_FILENO);
			write(STDOUT_FILENO, "=", 1);
			ft_putstr_fd(current->value, STDOUT_FILENO);
			write(STDOUT_FILENO, "\n", 1);
		}
		current = current->next;
	}
	return (0);
}

/**
 * @brief Prints environment variables in a format suitable for export.
 * 
 * Prints all environment variables in the format "declare -x NAME[=\"value\"]",
 * which is compatible with bash's export command output format. 
 * This is typically used by the export builtin when called with no arguments.
 * 
 * @param env Pointer to the head of the environment variables linked list
 */
void	print_env_vars(t_env *env)
{
	while (env)
	{
		ft_printf("declare -x %s", env->name);
		if (env->value)
			ft_printf("=\"%s\"", env->value);
		ft_printf("\n");
		env = env->next;
	}
}
