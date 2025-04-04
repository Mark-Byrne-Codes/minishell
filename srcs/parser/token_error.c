/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbyrne <mbyrne@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 11:12:02 by mbyrne            #+#    #+#             */
/*   Updated: 2025/04/04 10:05:55 by mbyrne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * @brief Checks if a token is an operator (pipe or redirection)
 * 
 * @param type The token type to check
 * @return int Returns 1 if operator, 0 otherwise
 */
int	is_operator(t_token_type type)
{
	if (type == TOKEN_PIPE
		|| (type >= TOKEN_REDIR_IN && type <= TOKEN_HEREDOC))
		return (1);
	return (0);
}

/**
 * @brief Gets the next non-whitespace token in the list
 * 
 * @param current_node Starting node in the token list
 * @return t_token* Pointer to next non-whitespace token, NULL if none found
 */
t_token	*get_next_non_ifs_token(t_list *current_node)
{
	t_list	*next_node;
	t_token	*next_token;

	next_node = current_node->next;
	while (next_node != NULL)
	{
		next_token = (t_token *)next_node->content;
		if (next_token->token != TOKEN_IFS)
			return (next_token);
		next_node = next_node->next;
	}
	return (NULL);
}

/**
 * @brief Validates if an operator has correct following token
 * 
 * @param current_node Node containing the operator token
 * @return int ERROR if invalid sequence, SUCCESS otherwise
 */
static int	validate_operator_sequence(t_list *current_node)
{
	t_token	*current_token;
	t_token	*next_token;

	current_token = (t_token *)current_node->content;
	next_token = get_next_non_ifs_token(current_node);
	if (next_token == NULL)
	{
		print_syntax_error(NULL);
		return (ERROR);
	}
	if (current_token->token == TOKEN_PIPE && next_token->token == TOKEN_PIPE)
	{
		print_syntax_error(next_token->string);
		return (ERROR);
	}
	if ((current_token->token >= TOKEN_REDIR_IN
			&& current_token->token <= TOKEN_HEREDOC)
		&& (next_token->token >= TOKEN_REDIR_IN
			&& next_token->token <= TOKEN_HEREDOC))
	{
		print_syntax_error(next_token->string);
		return (ERROR);
	}
	return (SUCCESS);
}

/**
 * @brief Validates token sequence in command parsing
 * 
 * @param token Current token being checked
 * @param current_node Node in token list
 * @param is_first Flag indicating if first token in command
 * @return int ERROR if invalid sequence, SUCCESS otherwise
 */
int	validate_token_sequence(t_token *token, t_list *current_node, int is_first)
{
	if (is_first && token->token == TOKEN_PIPE)
	{
		print_syntax_error(token->string);
		return (ERROR);
	}
	if (is_operator(token->token))
		return (validate_operator_sequence(current_node));
	return (SUCCESS);
}

/**
 * @brief Processes current token during command parsing
 * 
 * @param mini Main shell structure
 * @param current_node_ptr Pointer to current node in token list
 * @param idx Pointer to current command index
 * @return int ERROR if processing fails, SUCCESS otherwise
 */
int	process_current_token(t_mini *mini, t_list **current_node_ptr, int *idx)
{
	t_token	*token;
	t_list	*original_node;
	int		ret;

	if (current_node_ptr == NULL || *current_node_ptr == NULL)
		return (ERROR);
	original_node = *current_node_ptr;
	token = (t_token *)original_node->content;
	ret = process_token_parse(mini, token, current_node_ptr, idx);
	if (ret == ERROR)
		return (ERROR);
	if (*current_node_ptr == original_node)
		*current_node_ptr = (*current_node_ptr)->next;
	return (SUCCESS);
}
