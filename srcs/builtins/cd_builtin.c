/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_builtin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbyrne <mbyrne@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 09:17:19 by mbyrne            #+#    #+#             */
/*   Updated: 2025/03/09 13:24:43 by mbyrne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	print_cd_error(char *path)
{
	ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
	ft_putstr_fd(path, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putstr_fd(strerror(errno), STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
}

static void	update_env_variable(t_mini *mini, const char *var_name,
		const char *new_value)
{
	t_env	*env_node;
	char	*value_copy;
	t_env	*new_node;

	env_node = find_env_node(mini->env, var_name);
	value_copy = ft_strdup(new_value);
	if (env_node)
		env_node->value = value_copy;
	else
	{
		new_node = malloc(sizeof(t_env));
		new_node->name = ft_strdup(var_name);
		new_node->value = value_copy;
		new_node->next = NULL;
		add_env_node(&mini->env, new_node);
	}
}

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

int	cd_builtin(t_mini *mini, char **args)
{
	if (!args[1])
	{
		ft_putstr_fd("minishell: cd: missing argument\n", STDERR_FILENO);
		return (1);
	}
	return (change_directory(mini, args[1]));
}