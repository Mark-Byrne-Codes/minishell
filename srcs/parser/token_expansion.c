/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_expansion.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbyrne <mbyrne@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 09:59:13 by mbyrne            #+#    #+#             */
/*   Updated: 2025/04/01 09:07:52 by mbyrne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * @brief Determines if a variable should be expanded at current position
 * 
 * @param str Input string being processed
 * @param i Current position in string
 * @param in_quotes Quote state (0=none, 1=single, 2=double)
 * @return int 1 if should expand, 0 otherwise
 * 
 * @note Special cases:
 * - No expansion in single quotes
 * - Expands $? (exit status)
 * - Expands valid variable names (alnum + underscore)
 * - Expands $" in double quotes
 */
static int	should_expand_variable(char *str, int i, int in_quotes)
{
	if (in_quotes == 1)
		return (0);
	if (str[i] == '$')
	{
		if (str[i + 1] == '?' || ft_isalnum(str[i + 1]) || str[i + 1] == '_')
			return (1);
		if (in_quotes == 2 && str[i + 1] == '"')
			return (1);
	}
	return (0);
}

/**
 * @brief Handles the actual variable expansion process
 * 
 * @param mini Minishell context
 * @param str Original input string
 * @param res Result string being built
 * @param i Current position in string
 * @return int New position after expansion or -1 on error
 * 
 * @note Steps:
 * 1. Extracts variable name
 * 2. Gets variable value from environment
 * 3. Appends value to result
 * 4. Handles special $? case
 * 5. Returns updated position
 */
static int	handle_variable_expansion(t_mini *mini, char *str,
				char **res, int i)
{
	char	*var_name;
	char	*var_value;
	char	*temp;
	int		name_len;

	var_name = get_var_name(&str[i]);
	if (!var_name)
		return (-1);
	var_value = expand_env_var(mini, var_name);
	temp = *res;
	*res = ft_strjoin(*res, var_value);
	free(temp);
	free(var_value);
	name_len = ft_strlen(var_name);
	if (ft_strcmp(var_name, "?") == 0)
		name_len = 1;
	free(var_name);
	return (i + name_len + 1);
}

/**
 * @brief Appends a single character to the result string
 * 
 * @param result Pointer to result string
 * @param c Character to append
 * @return int 1 on success, 0 on memory failure
 * 
 * @note Handles memory allocation and copying safely
 * Frees old result buffer on reallocation
 */
static int	append_char(char **result, char c)
{
	char	*temp;
	size_t	result_len;
	char	*new_result;

	temp = *result;
	result_len = 0;
	if (temp)
		result_len = ft_strlen(temp);
	new_result = malloc(result_len + 2);
	if (!new_result)
	{
		free(temp);
		*result = NULL;
		return (0);
	}
	if (temp)
		ft_memcpy(new_result, temp, result_len);
	new_result[result_len] = c;
	new_result[result_len + 1] = '\0';
	*result = new_result;
	free(temp);
	return (1);
}

/**
 * @brief Main variable expansion function
 * 
 * @param mini Minishell context
 * @param str Input string to expand
 * @param in_quotes Current quote state
 * @return char* Expanded string or NULL on error
 * 
 * @note Processes string character by character:
 * - Expands variables when appropriate
 * - Preserves other characters
 * - Handles memory allocation and cleanup
 */
char	*expand_variables(t_mini *mini, char *str, int in_quotes)
{
	char	*result;
	int		i;

	result = ft_strdup("");
	if (!result)
		return (NULL);
	i = 0;
	while (str[i])
	{
		if (should_expand_variable(str, i, in_quotes))
		{
			i = handle_variable_expansion(mini, str, &result, i);
			if (i == -1)
				return (NULL);
		}
		else
		{
			if (!append_char(&result, str[i]))
				return (NULL);
			i++;
		}
	}
	return (result);
}
