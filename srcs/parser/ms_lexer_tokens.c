/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_lexer_tokens.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbyrne <mbyrne@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 00:27:13 by elehtone          #+#    #+#             */
/*   Updated: 2025/03/23 14:20:03 by mbyrne           ###   ########.fr       */
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
	while (cmd[len] != '\0' && !fun_check_ifs(cmd[len]) && !fun_check_any_quote(cmd[len]))
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
	int	len;

	len = 0;
	while (cmd[len] != '\0' && !fun_check_ifs(cmd[len]) && !fun_check_any_quote(cmd[len]))
		len++;
	cmd = fun_add_entry(lexer, cmd, len, TOKEN_WORD);
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
	start = command + 1; // Skip the opening quote
	len = 0;
	while (start[len] != '\0' && start[len] != '"')
		len++;

	// Extract the content between quotes
	content = ft_substr(start, 0, len);
	if (!content)
		return (NULL);

	// Add the content as a word token
	fun_add_entry(lexer, content, len, TOKEN_WORD);
	free(content);

	// Skip past the closing quote if we found one
	if (start[len] == '"')
	{
		fun_flag_flipper(&lexer->dquote);
		return (start + len + 1);
	}
	else
	{
		// No closing quote found
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
	start = command + 1; // Skip the opening quote
	len = 0;
	while (start[len] != '\0' && start[len] != '\'')
		len++;

	// Extract the content between quotes
	content = ft_substr(start, 0, len);
	if (!content)
		return (NULL);

	// Add the content as a word token
	fun_add_entry(lexer, content, len, TOKEN_WORD);
	free(content);

	// Skip past the closing quote if we found one
	if (start[len] == '\'')
	{
		fun_flag_flipper(&lexer->squote);
		return (start + len + 1);
	}
	else
	{
		// No closing quote found
		return (start + len);
	}
}
