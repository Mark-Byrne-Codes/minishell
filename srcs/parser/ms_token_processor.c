/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_token_processor.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbyrne <mbyrne@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 15:30:12 by mbyrne            #+#    #+#             */
/*   Updated: 2025/03/24 15:45:51 by mbyrne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	handle_word_var(t_mini *mini, t_command *cmd, t_token *data, int *idx)
{
	char	*exp;
	int		result;

	exp = expand_token(mini, data);
	if (!exp)
		return (ERROR);
	result = SUCCESS;
	if (exp[0] || *idx > 0)
	{
		if (add_argument(cmd, exp, *idx) == ERROR)
			result = ERROR;
		else
		{
			if (*idx == 0 && data->token == TOKEN_WORD && is_builtin(exp))
				cmd->is_builtin = 1;
			(*idx)++;
		}
	}
	free(exp);
	return (result);
}

int	handle_single_quote(t_command *cmd, t_token *data, int *idx)
{
	char	*content;

	content = ft_strdup(data->string);
	if (!content)
		return (ERROR);
	if (add_argument(cmd, content, *idx) == ERROR)
	{
		free(content);
		return (ERROR);
	}
	if (*idx == 0)
		cmd->is_builtin = is_builtin(content);
	(*idx)++;
	free(content);
	return (SUCCESS);
}

static int	concat_tokens(t_token *d1, t_token *d2, t_list *t1, t_list *t2)
{
	char	*new;

	new = ft_strjoin(d1->string, d2->string);
	if (!new)
		return (ERROR);
	free(d1->string);
	d1->string = new;
	t1->next = t2->next;
	free(d2->string);
	free(d2);
	free(t2);
	return (SUCCESS);
}

int	concat_adjacent_strings(t_lexer *lexer)
{
	t_list	*tmp;
	t_token	*data;
	t_token	*nx;

	tmp = lexer->dictionary;
	while (tmp && tmp->next)
	{
		data = (t_token *)tmp->content;
		nx = (t_token *)tmp->next->content;
		if ((data->token == TOKEN_WORD || data->token == TOKEN_SINGLE_Q_STRING)
			&& (nx->token == TOKEN_WORD || nx->token == TOKEN_SINGLE_Q_STRING))
		{
			if (concat_tokens(data, nx, tmp, tmp->next) == ERROR)
				return (ERROR);
		}
		else
			tmp = tmp->next;
	}
	return (SUCCESS);
}
