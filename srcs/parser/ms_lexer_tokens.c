/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_lexer_tokens.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elehtone <elehtone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 00:27:13 by elehtone          #+#    #+#             */
/*   Updated: 2025/03/15 01:02:02 by elehtone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
 * Handle strings contained within single quotes.
 * Params: *lexer, *command
 * Return: address of the first character after the quoted string ends
 */
void	*fun_variable_string(t_lexer *lexer, char *cmd)
{
	int	len;

	len = 0;
	while (cmd[len] != '\0' && cmd[len] != '"' && !fun_check_ifs(cmd[len]))
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
	while (cmd[len] != '\0' && cmd[len] != '"' && !fun_check_ifs(cmd[len]))
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
	fun_flag_flipper(&lexer->dquote);
	fun_add_entry(lexer, command, 1, TOKEN_DOUBLE_Q);
	command++;
	while (*command != '\0' && *command != 34)
	{
		if (*command == '$')
			command = fun_variable_string(lexer, command);
		else if (fun_check_ifs(*command))
			command = fun_add_entry(lexer, command, 1, TOKEN_IFS);
		else
			command = fun_word_string(lexer, command);
	}
	if (*command == 34)
	{
		fun_flag_flipper(&lexer->dquote);
		command = fun_add_entry(lexer, command, 1, TOKEN_DOUBLE_Q);
	}
	return (command);
}

/*
 * Handle strings contained within single quotes.
 * Params: *lexer, *command
 * Return: address of the first character after the quoted string ends
 */
void	*fun_single_quote_string(t_lexer *lexer, char *command)
{
	int	len;

	fun_flag_flipper(&lexer->squote);
	len = 1;
	while (command[len] != '\0' && lexer->squote == 1)
	{
		if (command[len] == 39)
			fun_flag_flipper(&lexer->squote);
		len++;
	}
	command = fun_add_entry(lexer, command, len, TOKEN_SINGLE_Q_STRING);
	return (command);
}
