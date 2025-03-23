/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_parser.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbyrne <mbyrne@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 12:33:39 by mbyrne            #+#    #+#             */
/*   Updated: 2025/03/23 13:16:36 by mbyrne           ###   ########.fr       */
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

static int	handle_redirection_parse(t_mini *mini, t_command *cmd, t_list **temp)
{
	if (ms_handle_redirection(mini, cmd, temp) == ERROR)
		return (ERROR);
	return (SUCCESS);
}

static int	handle_word_or_variable(t_mini *mini, t_command *cmd, 
		t_token *data, int *arg_idx)
{
	if (ms_handle_token_expansion(mini, cmd, data, arg_idx) == ERROR)
		return (ERROR);
	return (SUCCESS);
}

static int	handle_ifs(t_list **temp)
{
	*temp = (*temp)->next;
	return (SUCCESS);
}

static int	process_token(t_mini *mini, t_token *data, t_list **temp, 
		int *cmd_idx, int *arg_idx)
{
	if (data->token == TOKEN_PIPE)
	{
		if (handle_pipe(temp, cmd_idx, arg_idx) == ERROR)
			return (ERROR);
	}
	else if (data->token >= TOKEN_REDIR_IN && data->token <= TOKEN_HEREDOC)
	{
		if (handle_redirection_parse(mini, &mini->commands[*cmd_idx], temp) == ERROR)
			return (ERROR);
	}
	else if (data->token == TOKEN_WORD || data->token == TOKEN_VARIABLE)
	{
		if (handle_word_or_variable(mini, &mini->commands[*cmd_idx], 
				data, arg_idx) == ERROR)
			return (ERROR);
	}
	else if (data->token == TOKEN_IFS)
	{
		if (handle_ifs(temp) == ERROR)
			return (ERROR);
	}
	return (SUCCESS);
}

int	ms_process_tokens(t_mini *mini, t_lexer *lexer)
{
	t_list	*temp;
	t_token	*data;
	int		cmd_idx;
	int		arg_idx;

	temp = lexer->dictionary;
	cmd_idx = 0;
	arg_idx = 0;
	while (temp)
	{
		data = (t_token *)temp->content;
		if (process_token(mini, data, &temp, &cmd_idx, &arg_idx) == ERROR)
			return (ERROR);
		if (data->token != TOKEN_PIPE && data->token != TOKEN_IFS
			&& !(data->token >= TOKEN_REDIR_IN && data->token <= TOKEN_HEREDOC))
			temp = temp->next;
	}
	return (SUCCESS);
}
