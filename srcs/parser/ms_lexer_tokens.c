/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_lexer_tokens.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbyrne <mbyrne@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 00:27:13 by elehtone          #+#    #+#             */
/*   Updated: 2025/03/26 10:56:43 by mbyrne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*
 * Handle strings contained within single quotes.
 * Params: *lexer, *command
 * Return: address of the first character after the quoted string ends
 */
void	*fun_variable_string(t_lexer *lexer, char *cmd)
{
	int	len;

	len = 0;
	while (cmd[len] != '\0' && !fun_check_ifs(cmd[len])
		&& !fun_check_any_quote(cmd[len]))
		len++;
	cmd = fun_add_entry(lexer, cmd, len, TOKEN_VARIABLE);
	lexer->vars++;
	return (cmd);
}

/*
 * Handle plain words (commands and such with no prepositions and not contained
 * 	in any quotes)
 * Params: *lexer, *command
 * Returns: current location within command
 */
void	*fun_word_string(t_lexer *lexer, char *cmd)
{
	int	i;

	i = 0;
	while (cmd[i] && !fun_check_ifs(cmd[i]) && !fun_check_any_quote(cmd[i])
		&& cmd[i] != '$' && cmd[i] != '|' && cmd[i] != '<' && cmd[i] != '>')
		i++;
	if (i > 0)
		cmd = fun_add_entry(lexer, cmd, i, TOKEN_WORD);
	return (cmd);
}

/*
 * Handle strings contained within double quotes.
 * Params: *lexer, *command
 * Return: address of the first character after the quoted string ends
 */
void	*fun_double_quote_string(t_lexer *lexer, char *command)
{
	char *start;
	int len;
	char *content;

	fun_flag_flipper(&lexer->dquote);
	start = command + 1;
	len = 0;
	while (start[len] != '\0' && start[len] != '"')
		len++;
	content = ft_substr(start, 0, len);
	if (!content)
		return (NULL);
	fun_add_entry(lexer, content, len, TOKEN_WORD);
	free(content);
	if (start[len] == '"')
	{
		fun_flag_flipper(&lexer->dquote);
		return (start + len + 1);
	}
	else
	{
		return (start + len);
	}
}

/*
 * Handle strings contained within single quotes.
 * Params: *lexer, *command
 * Return: address of the first character after the quoted string ends
 */
void	*fun_single_quote_string(t_lexer *lexer, char *command)
{
	char *start;
	int len;
	char *content;

	fun_flag_flipper(&lexer->squote);
	start = command + 1;
	len = 0;
	while (start[len] != '\0' && start[len] != '\'')
		len++;
	content = ft_substr(start, 0, len);
	if (!content)
		return (NULL);
	fun_add_entry(lexer, content, len, TOKEN_SINGLE_Q_STRING);
	free(content);
	if (start[len] == '\'')
	{
		fun_flag_flipper(&lexer->squote);
		return (start + len + 1);
	}
	else
	{
		return (start + len);
	}
}

/*
 * Checks if a character is either a single or double quote.
 * Param: char
 * Return: 0 if not, 1 if single, 2 if double
 */
int	fun_check_any_quote(unsigned int c)
{
	if (c == 39)
		return (1);
	else if (c == 34)
		return (2);
	return (0);
}