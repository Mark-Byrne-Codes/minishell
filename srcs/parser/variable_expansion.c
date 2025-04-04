/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_expansion.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbyrne <mbyrne@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 09:54:23 by mbyrne            #+#    #+#             */
/*   Updated: 2025/04/01 09:15:08 by mbyrne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * @brief Main token expansion function
 * 
 * @param mini Minishell context
 * @param token Token to expand
 * @return char* Expanded string or NULL on error
 * 
 * @note Handles:
 * - Tilde expansion
 * - Variable expansion (except in single quotes)
 * - Exit status expansion ($?)
 * Marks token as expanded when done
 */
char	*expand_token(t_mini *mini, t_token *token)
{
	char	*expanded;
	char	*temp;

	if (token->token == TOKEN_SINGLE_Q_STRING)
		return (ft_strdup(token->string));
	expanded = ft_strdup(token->string);
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

/**
 * @brief Extracts variable name from string
 * 
 * @param str String starting with $
 * @return char* Extracted variable name or NULL
 * 
 * @note Handles special $? case and validates:
 * - Alphanumeric characters
 * - Underscores
 * Returns NULL for invalid names
 */
char	*get_var_name(const char *str)
{
	int		i;
	char	*name;

	i = 0;
	str++;
	if (str[0] == '?')
		return (ft_strdup("?"));
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	if (i == 0)
		return (NULL);
	name = ft_substr(str, 0, i);
	if (!name)
		return (NULL);
	return (name);
}

/**
 * @brief Expands $? to exit status string
 * 
 * @param mini Minishell context
 * @return char* String representation of exit status
 * 
 * @note Handles global exit status if set,
 * otherwise uses mini->exit_status
 */
char	*expand_exit_status(t_mini *mini)
{
	char	*status_str;

	if (g_signal != 0)
		mini->exit_status = g_signal;
	g_signal = 0;
	status_str = ft_itoa(mini->exit_status);
	if (!status_str)
		return (ft_strdup(""));
	return (status_str);
}

/**
 * @brief Expands environment variable by name
 * 
 * @param mini Minishell context
 * @param var_name Variable name to expand
 * @return char* Variable value or empty string if not found
 * 
 * @note Special handling for $? (exit status)
 * Returns empty string for undefined variables
 */
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
