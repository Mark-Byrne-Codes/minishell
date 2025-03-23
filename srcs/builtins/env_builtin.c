/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_builtin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbyrne <mbyrne@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 09:47:16 by mbyrne            #+#    #+#             */
/*   Updated: 2025/03/23 12:03:09 by mbyrne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
