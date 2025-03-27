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

/*
 * Adds a new entry to the dictionary. Creates pointers to a new token and node
 * 	fills the token's data and creates the new node then adds it to the end
 * 	of the current list.
 * Params: lexer, *string, len, token
 * Returns: nothing yet
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

/*
 * Changes the value of a flag. Ghetto bool for stick in the muds.
 * Params: flag
 */
void	toggle_quote_state(int *flag)
{
	if (*flag == 0)
		*flag = 1;
	else
		*flag = 0;
}

/*
 * Checks if a character is an input field separator. Generally these are
 * 	space, tab, new line.
 * Param: char
 * Return: 0 if not, 1 if
 */
int	is_whitespace_character(unsigned int c)
{
	if (c == ' ' || c == 9 || c == 10)
		return (1);
	return (0);
}

/*
 * Clean up the lexer data after expected or unexpected end.
 * Rolls through the list, freeing the token's string then clears the rest of
 * the list. Exits with failure code.
 * params: *lexer, end flag (1 to exit fail, 0 to not)
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
