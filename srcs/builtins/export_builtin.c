/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_builtin.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbyrne <mbyrne@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 09:46:19 by mbyrne            #+#    #+#             */
/*   Updated: 2025/03/23 12:19:02 by mbyrne           ###   ########.fr       */
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
			ft_putstr_fd("minishell: export: `", STDERR_FILENO);
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

int	export_builtin(t_mini *mini, char **args)
{
	int	i;
	int	ret;

	if (!args[1])
	{
		print_env_vars(mini->env);
		return (0);
	}
	i = 1;
	ret = 0;
	while (args[i])
	{
		if (set_env_var_internal(mini, args[i]) != 0)
			ret = 1;
		i++;
	}
	return (ret);
}
