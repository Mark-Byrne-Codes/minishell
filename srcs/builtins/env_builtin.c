/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_builtin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbyrne <mbyrne@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 09:47:16 by mbyrne            #+#    #+#             */
/*   Updated: 2025/03/06 12:31:20 by mbyrne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_env(t_mini *mini)
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