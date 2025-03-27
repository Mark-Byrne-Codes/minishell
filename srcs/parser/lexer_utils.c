/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbyrne <mbyrne@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 23:17:17 by elehtone          #+#    #+#             */
/*   Updated: 2025/03/27 11:44:42 by mbyrne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * @brief Adds new entry to dictionary
 * 
 * @param lexer Lexer data struct type: t_lexer
 * @param str Command string
 * @param len Length of string to add
 * @param token Enum token types (needed?)
 * @return void* Pointer to current location in command
 * 
 * @note Adds entry from command string
 * - Length defined by caller
 */
void	*add_token_to_list(t_lexer *lexer, char *str, int len,
		t_token_type token)
{
	char	*new_string;
	t_token	*new_token;
	t_list	*new_node;

	new_string = ft_substr(str, 0, len);
	if (!new_string)
		cleanup_on_token_error(lexer, 1);
	new_token = malloc(sizeof(t_token));
	if (!new_token)
	{
		free(new_string);
		cleanup_on_token_error(lexer, 1);
	}
	new_token->token = token;
	new_token->string = new_string;
	new_token->len = len;
	new_node = ft_lstnew(new_token);
	if (!new_node)
	{
		free_token(new_token);
		cleanup_on_token_error(lexer, 1);
	}
	ft_lstadd_back(&lexer->dictionary, new_node);
	return (str + len);
}

/**
 * @brief Changes the value of a flag
 * 
 * @param *flag Pointer to int flag
 * 
 * @note Either 0 or 1. Switches when called.
 * 
 */
void	toggle_quote_state(int *flag)
{
	if (*flag == 0)
		*flag = 1;
	else
		*flag = 0;
}

/**
 * @brief Checks if a character is an input field separator.
 * 
 * @param c Value to check
 * @return int 1 if IFS, 0 if not
 * 
 * @note As it is default is space, tab, new line
 * - Could be adapted to use $IFS if needed
 * 
 */
int	is_whitespace_character(unsigned int c)
{
	if (c == ' ' || c == 9 || c == 10)
		return (1);
	return (0);
}

/**
 * @brief Clean up the lexer data. 
 * 
 * @param lexer Lexer data struct type: t_lexer
 * @param end 1 if exit required
 * 
 * @note Used after expected or unexpected end.
 */
void	cleanup_on_token_error(t_lexer *lexer, int end)
{
	t_list	*temp;
	t_token	*token;

	temp = lexer->dictionary;
	while (temp != NULL)
	{
		token = (t_token *)temp->content;
		free(token->string);
		temp = temp->next;
	}
	ft_lstclear(&lexer->dictionary, free);
	if (end == 1)
		exit(EXIT_FAILURE);
}
