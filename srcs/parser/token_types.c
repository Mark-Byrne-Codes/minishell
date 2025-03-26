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
void	*handle_variable_token(t_lexer *lexer, char *cmd)
{
	int	len;

	len = 0;
	while (cmd[len] != '\0' && !is_whitespace_character(cmd[len])
		&& !is_quote_character(cmd[len]))
		len++;
	cmd = add_token_to_list(lexer, cmd, len, TOKEN_VARIABLE);
	lexer->vars++;
	return (cmd);
}

/*
 * Handle plain words (commands and such with no prepositions and not contained
 * 	in any quotes)
 * Params: *lexer, *command
 * Returns: current location within command
 */
void	*handle_word_token(t_lexer *lexer, char *cmd)
{
	int	i;

	i = 0;
	while (cmd[i] && !is_whitespace_character(cmd[i])
		&& !is_quote_character(cmd[i])
		&& cmd[i] != '$' && cmd[i] != '|'
		&& cmd[i] != '<' && cmd[i] != '>')
		i++;
	if (i > 0)
		cmd = add_token_to_list(lexer, cmd, i, TOKEN_WORD);
	return (cmd);
}

/*
 * Handle strings contained within double quotes.
 * Params: *lexer, *command
 * Return: address of the first character after the quoted string ends
 */
void	*handle_double_quoted_string(t_lexer *lexer, char *cmd)
{
	char	*start;
	int		len;
	char	*content;

	toggle_quote_state(&lexer->dquote);
	start = cmd + 1;
	len = 0;
	while (start[len] != '\0' && start[len] != '"')
		len++;
	content = ft_substr(start, 0, len);
	if (!content)
		return (NULL);
	add_token_to_list(lexer, content, len, TOKEN_WORD);
	free(content);
	if (start[len] == '"')
		toggle_quote_state(&lexer->dquote);
	if (start[len] == '"')
		return (start + len + 1);
	else
		return (start + len);
}

/*
 * Handle strings contained within single quotes.
 * Params: *lexer, *command
 * Return: address of the first character after the quoted string ends
 */
void	*handle_single_quoted_string(t_lexer *lexer, char *cmd)
{
	char	*start;
	int		len;
	char	*content;

	toggle_quote_state(&lexer->squote);
	start = cmd + 1;
	len = 0;
	while (start[len] != '\0' && start[len] != '\'')
		len++;
	content = ft_substr(start, 0, len);
	if (!content)
		return (NULL);
	add_token_to_list(lexer, content, len, TOKEN_SINGLE_Q_STRING);
	free(content);
	if (start[len] == '\'')
		toggle_quote_state(&lexer->squote);
	if (start[len] == '\'')
		return (start + len + 1);
	else
		return (start + len);
}
