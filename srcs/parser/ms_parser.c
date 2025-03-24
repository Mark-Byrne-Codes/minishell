/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_parser.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbyrne <mbyrne@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 12:33:39 by mbyrne            #+#    #+#             */
/*   Updated: 2025/03/24 09:23:51 by mbyrne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	handle_pipe(t_list **temp, int *cmd_idx, int *arg_idx)
{
	(*cmd_idx)++;
	*arg_idx = 0;
	*temp = (*temp)->next;
	return (SUCCESS);
}

static int	handle_redirection_parser(t_mini *mini, t_command *cmd,
	t_list **temp)
{
	if (ms_handle_redirection(mini, cmd, temp) == ERROR)
		return (ERROR);
	return (SUCCESS);
}

static int	process_token(t_mini *mini, t_token *data, t_list **temp,
			int *idx)
{
	int	*cmd_idx;
	int	*arg_idx;

	cmd_idx = &idx[0];
	arg_idx = &idx[1];
	if (data->token == TOKEN_PIPE)
		return (handle_pipe(temp, cmd_idx, arg_idx));
	else if (data->token >= TOKEN_REDIR_IN && data->token <= TOKEN_HEREDOC)
		return (handle_redirection_parser(mini,
				&mini->commands[*cmd_idx], temp));
	else if (data->token == TOKEN_WORD || data->token == TOKEN_VARIABLE)
		return (handle_word_var(mini, &mini->commands[*cmd_idx],
				data, arg_idx));
	else if (data->token == TOKEN_SINGLE_Q_STRING)
		return (handle_single_quote(&mini->commands[*cmd_idx], data, arg_idx));
	else if (data->token == TOKEN_IFS)
		*temp = (*temp)->next;
	return (SUCCESS);
}

static int	process_token_list(t_mini *mini, t_lexer *lexer)
{
	t_list	*temp;
	t_token	*data;
	int		idx[2];

	temp = lexer->dictionary;
	idx[0] = 0;
	idx[1] = 0;
	while (temp)
	{
		data = (t_token *)temp->content;
		if (process_token(mini, data, &temp, idx) == ERROR)
			return (ERROR);
		if (data->token != TOKEN_PIPE && data->token != TOKEN_IFS
			&& !(data->token >= TOKEN_REDIR_IN && data->token <= TOKEN_HEREDOC))
			temp = temp->next;
	}
	return (SUCCESS);
}

int	ms_process_tokens(t_mini *mini, t_lexer *lexer)
{
	if (concat_adjacent_strings(lexer) == ERROR)
		return (ERROR);
	return (process_token_list(mini, lexer));
}
