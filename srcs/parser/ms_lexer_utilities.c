/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_lexer_utilities.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elehtone <elehtone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 23:17:17 by elehtone          #+#    #+#             */
/*   Updated: 2025/03/14 13:04:54 by elehtone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*
 * Adds a new entry to the dictionary. Creates pointers to a new token and node
 * 	fills the token's data and creates the new node then adds it to the end
 * 	of the current list.
 * Params: lexer, *string, len, token
 * Returns: nothing yet
 */
void	*fun_add_entry(t_lexer *lexer, char *str, int len, t_token_type token)
{
	char	*new_string;
	t_token	*new_token;
	t_list	*new_node;

	new_string = ft_substr(str, 0, len);
	new_token = malloc(sizeof(t_token));
	new_token->token = token;
	new_token->string = new_string;
	new_token->len = len;
	new_node = ft_lstnew(new_token);
	ft_lstadd_back(&lexer->dictionary, new_node);
	return (str + len);
}

/*
 * Initialise the lexer struct with default values of 0.
 * Initialise the dictionary list node to NULL.
 * Params: lexer
 */
void	fun_lex_struct_init(t_lexer *lexer)
{
	lexer->squote = 0;
	lexer->dquote = 0;
	lexer->pipes = 0;
	lexer->vars = 0;
	lexer->red_in = 0;
	lexer->red_delim = 0;
	lexer->red_out = 0;
	lexer->red_append = 0;
	lexer->dictionary = NULL;
}

/*
 * Changes the value of a flag. Ghetto bool for stick in the muds.
 * Params: flag
 */
void	fun_flag_flipper(int *flag)
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
int	fun_check_ifs(unsigned int c)
{
	if (c == ' ' || c == 9 || c == 10)
		return (1);
	return (0);
}
