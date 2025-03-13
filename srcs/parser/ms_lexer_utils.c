/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_lexer_utilities.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elehtone <elehtone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 23:17:17 by elehtone          #+#    #+#             */
/*   Updated: 2025/03/10 18:02:24 by elehtone         ###   ########.fr       */
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
int	fun_add_entry(t_lexer *lexer, char *string, int len, t_token_type token)
{
	t_token	*new_token;
	t_list	*new_node;

	new_token = malloc(sizeof(t_token));
	new_token->token = token;
	new_token->string = string;
	new_token->len = len;
	new_node = ft_lstnew(new_token);
	ft_lstadd_back(&lexer->dictionary, new_node);
	return (0);
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

/*
 * Count the kind of redirect. Helper to fun_count_special_chars()
 * Params: *lexer, *str
 * Returns: str (address of current string location)
 
static void	*fun_count_redirect_chars(t_lexer *lexer, char *str)
{
	if (*str == '<')
	{
		if (*(str + 1) == '<')
		{
			lexer->red_delim++;
			str++;
		}
		else
			lexer->red_in++;
	}
	else if (*str == '>')
	{
		if (*(str + 1) == '>')
		{
			lexer->red_append++;
			str++;
		}
		else
		lexer->red_out++;
	}
	str++;
	return (str);
}


 * Count a number of characters that are not included in single or double quotes.
 * Params: *lexer, *str
 * Returns: int (number of unbounded character)
 *
static int	fun_count_special_chars(t_lexer *lexer, char *str)
{
	while (*str)
	{
		if (*str == 39)
			fun_flag_flipper(&lexer->single_quote_flag);
		else if (*str == 34)
			fun_flag_flipper(&lexer->double_quote_flag);
		else if (*str == '$' && !lexer->single_quote_flag)
			lexer->vars++;
		else if (*str == '|' && !lexer->single_quote_flag && !lexer->double_quote_flag)
			lexer->pipes++;
		else if ((*str == '<' || *str == '>') && !lexer->single_quote_flag
				&& !lexer->double_quote_flag)
		{
			str = fun_count_redirect_chars(lexer, str);
		}
		str++;
	}
	return (0);
}*/
