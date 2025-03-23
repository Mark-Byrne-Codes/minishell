/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_expand_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbyrne <mbyrne@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 09:54:23 by mbyrne            #+#    #+#             */
/*   Updated: 2025/03/23 13:22:46 by mbyrne           ###   ########.fr       */
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
char *expand_token(t_mini *mini, t_token *token)
{
    // Check if the string contains single quotes (regardless of token type)
    if (token->string && token->string[0] == '\'' && 
        token->string[ft_strlen(token->string) - 1] == '\'')
    {
        // Extract content between quotes without expanding variables
        return ft_substr(token->string, 1, ft_strlen(token->string) - 2);
    }

    // Handle double-quoted strings
    if (token->string && token->string[0] == '"' && 
        token->string[ft_strlen(token->string) - 1] == '"')
    {
        // Extract and expand content between double quotes
        char *content = ft_substr(token->string, 1, ft_strlen(token->string) - 2);
        if (!content)
            return NULL;
        
        char *temp = content;
        char *expanded = expand_variables(mini, content, 2); // 2 for double quotes
        free(temp);
        return expanded;
    }

    // Normal token - expand variables and tildes
    char *expanded = expand_tilde(mini, token->string);
    if (!expanded)
        return NULL;
    
    char *temp = expanded;
    expanded = expand_variables(mini, expanded, 0); // 0 for no quotes
    free(temp);
    
    token->expanded = 1;
    return expanded;
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
	
	// Extract just the variable name
	name = ft_substr(str, 0, i);
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
	
	// Handle common environment variables explicitly
	if (ft_strcmp(var_name, "USER") == 0)
	{
		env_var = find_env_node(mini->env, "USER");
		if (env_var && env_var->value)
			return (ft_strdup(env_var->value));
	}
	else if (ft_strcmp(var_name, "HOME") == 0)
	{
		env_var = find_env_node(mini->env, "HOME");
		if (env_var && env_var->value)
			return (ft_strdup(env_var->value));
	}
	
	// General case
	env_var = find_env_node(mini->env, var_name);
	if (!env_var || !env_var->value)
		return (ft_strdup(""));
	
	value = ft_strdup(env_var->value);
	if (!value)
		return (ft_strdup(""));
	
	return (value);
}