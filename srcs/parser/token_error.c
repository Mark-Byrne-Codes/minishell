/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbyrne <mbyrne@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 11:12:02 by mbyrne            #+#    #+#             */
/*   Updated: 2025/04/01 11:48:32 by mbyrne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	is_operator(t_token_type type)
{
	return (type == TOKEN_PIPE || (type >= TOKEN_REDIR_IN
			&& type <= TOKEN_HEREDOC));
}

t_token	*get_next_non_ifs_token(t_list *current_node)
{
	t_list	*next_node;
	t_token	*next_token;

	next_node = current_node->next;
	while (next_node)
	{
		next_token = (t_token *)next_node->content;
		if (next_token->token != TOKEN_IFS)
			return (next_token);
		next_node = next_node->next;
	}
	return (NULL);
}

static int	validate_operator_sequence(t_list *current_node)
{
	t_token	*next_token;

	next_token = get_next_non_ifs_token(current_node);
	if (!next_token || (is_operator(next_token->token) && \
		next_token->token != TOKEN_REDIR_OUT))
	{
		if (next_token)
		{
			print_syntax_error(next_token->string);
		}
		else
		{
			print_syntax_error(NULL);
		}
		return (ERROR);
	}
	return (SUCCESS);
}

int	validate_token_sequence(t_token *token, t_list *current_node, int is_first)
{
	if (is_first && token->token == TOKEN_PIPE)
	{
		print_syntax_error(token->string);
		return (ERROR);
	}
	if (token->token == TOKEN_PIPE || (is_operator(token->token) && \
		token->token != TOKEN_REDIR_OUT))
		return (validate_operator_sequence(current_node));
	return (SUCCESS);
}

int	process_current_token(t_mini *mini, t_list **current_node_ptr, int *idx)
{
	t_token	*token;
	t_list	*original_node;
	int		ret;

	if (!current_node_ptr || !*current_node_ptr)
	{
		return (ERROR);
	}
	original_node = *current_node_ptr;
	token = (t_token *)original_node->content;
	ret = process_token_parse(mini, token, current_node_ptr, idx);
	if (ret == ERROR)
		return (ERROR);
	if (*current_node_ptr == original_node)
		*current_node_ptr = (*current_node_ptr)->next;
	return (SUCCESS);
}
