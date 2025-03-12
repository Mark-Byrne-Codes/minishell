/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_builtin.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbyrne <mbyrne@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 09:46:19 by mbyrne            #+#    #+#             */
/*   Updated: 2025/03/12 11:28:00 by mbyrne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	print_env_vars(t_env *env)
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

static int	is_valid_identifier(const char *name)
{
	if (!ft_isalpha(*name) && *name != '_')
		return (0);
	while (*name)
	{
		if (!ft_isalnum(*name) && *name != '_')
			return (0);
		name++;
	}
	return (1);
}

static void	set_env_var(t_mini *mini, const char *arg)
{
	char	*equal_sign;
	char	*name;
	char	*value;

	equal_sign = ft_strchr(arg, '=');
	if (equal_sign)
	{
		name = ft_strndup(arg, equal_sign - arg);
		value = ft_strdup(equal_sign + 1);
	}
	else
	{
		name = ft_strdup(arg);
		value = NULL;
	}
	if (!is_valid_identifier(name))
	{
		ft_printf("minishell: export: `%s': not a valid identifier\n", arg);
		free(name);
		free(value);
		return ;
	}
	update_env_variable(mini, name, value);
	free(name);
	free(value);
}

int	export_builtin(t_mini *mini, char **args)
{
	int	i;

	if (!args[1])
	{
		print_env_vars(mini->env);
	}
	else
	{
		i = 1;
		while (args[i])
		{
			set_env_var(mini, args[i]);
			i++;
		}
	}
	return (0);
}