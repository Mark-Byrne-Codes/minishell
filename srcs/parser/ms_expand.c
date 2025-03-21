/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_expand.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbyrne <mbyrne@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 09:59:13 by mbyrne            #+#    #+#             */
/*   Updated: 2025/03/21 17:11:15 by mbyrne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"


static char	*init_result(void)
{
	char	*result;

	result = ft_strdup("");
	if (!result)
		return (NULL);
	return (result);
}

static int	should_expand_variable(char *str, int i, int in_quotes)
{
	return (str[i] == '$' && (ft_isalpha(str[i + 1]) || str[i + 1] == '_'
			|| str[i + 1] == '?' || (in_quotes && str[i + 1] == '"')));
}

static int	handle_variable_expansion(t_mini *mini, char *str, char **res, int i)
{
	char	*var_name;
	char	*var_value;
	char	*temp;
	int		advance;

	var_name = get_var_name(&str[i]);
	if (!var_name)
		return (-1);
	var_value = expand_env_var(mini, var_name);
	temp = *res;
	*res = ft_strjoin(*res, var_value);
	free(temp);
	free(var_name);
	advance = ft_strlen(var_name) + 1;
	if (var_name[0] == '?')
		advance--;
	free(var_value);
	return (i + advance);
}

static int	append_char(char **result, char c)
{
	char	*temp;

	temp = *result;
	*result = ft_strjoinchar(*result, c);
	free(temp);
	if (!*result)
		return (0);
	return (1);
}

char	*expand_variables(t_mini *mini, char *str, int in_quotes)
{
	char	*result;
	int		i;

	result = init_result();
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
