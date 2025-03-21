/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_expand_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbyrne <mbyrne@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 09:54:23 by mbyrne            #+#    #+#             */
/*   Updated: 2025/03/21 09:55:29 by mbyrne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*expand_tilde(t_mini *mini, char *str)
{
	char	*home_dir;
	char	*result;

	if (str[0] != '~' || (str[1] != '\0' && str[1] != '/'))
		return (ft_strdup(str));
	home_dir = get_env_value(mini->env, "HOME");
	if (!home_dir)
		return (ft_strdup(str));
	if (str[1] == '\0')
		return (ft_strdup(home_dir));
	result = ft_strjoin(home_dir, str + 1);
	if (!result)
		return (ft_strdup(str));
	return (result);
}

// Main expansion function that handles all types of expansions
char	*expand_token(t_mini *mini, t_token *token)
{
	char	*expanded;
	char	*temp;

	if (token->token == TOKEN_SINGLE_Q_STRING)
		return (ft_strdup(token->string));
	expanded = expand_tilde(mini, token->string);
	if (!expanded)
		return (NULL);
	if (token->token != TOKEN_SINGLE_Q_STRING)
	{
		temp = expanded;
		expanded = expand_variables(mini, expanded,
				token->token == TOKEN_DOUBLE_Q_CONTENT);
		free(temp);
		if (!expanded)
			return (NULL);
	}
	token->expanded = 1;
	return (expanded);
}

char	*get_var_name(const char *str)
{
	int		i;
	char	*name;

	i = 0;
	// Skip $ character
	str++;
	// Handle special case of $? for exit status
	if (str[0] == '?')
		return (ft_strdup("?"));
	// Count length of variable name
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	if (i == 0)
		return (NULL);
	name = ft_strndup(str, i);
	return (name);
}

char	*expand_exit_status(t_mini *mini)
{
	char	*status_str;

	status_str = ft_itoa(mini->exit_status);
	if (!status_str)
		return (ft_strdup(""));
	return (status_str);
}

char	*expand_env_var(t_mini *mini, const char *var_name)
{
	t_env	*env_var;
	char	*value;

	if (ft_strcmp(var_name, "?") == 0)
		return (expand_exit_status(mini));
	env_var = find_env_node(mini->env, var_name);
	if (!env_var || !env_var->value)
		return (ft_strdup(""));
	value = ft_strdup(env_var->value);
	if (!value)
		return (ft_strdup(""));
	return (value);
}