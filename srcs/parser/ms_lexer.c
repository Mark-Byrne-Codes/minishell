/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_lexer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbyrne <mbyrne@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 15:09:57 by elehtone          #+#    #+#             */
/*   Updated: 2025/03/24 09:02:19 by mbyrne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	*fun_less_greater(t_lexer *lex, char *cmd)
{
	if (*cmd == '<' && *(cmd + 1) == '<' && !lex->squote && !lex->dquote)
	{
		cmd = fun_add_entry(lex, cmd, 2, TOKEN_HEREDOC);
		lex->red_delim++;
	}
	else if (*cmd == '>' && *(cmd + 1) == '>' && !lex->squote && !lex->dquote)
	{
		cmd = fun_add_entry(lex, cmd, 2, TOKEN_REDIR_APPEND);
		lex->red_append++;
	}
	else if (*cmd == '<' && !lex->squote && !lex->dquote)
	{
		cmd = fun_add_entry(lex, cmd, 1, TOKEN_REDIR_IN);
		lex->red_in++;
	}
	else if (*cmd == '>' && !lex->squote && !lex->dquote)
	{
		cmd = fun_add_entry(lex, cmd, 1, TOKEN_REDIR_OUT);
		lex->red_out++;
	}
	else
		return (NULL);
	while (*cmd && fun_check_ifs(*cmd))
		cmd++;
	return (cmd);
}

/*
 * Check the current character and act appropriately.
 * Params: *lexer, *command
 * Return: current location in *command
 */
void	*fun_charwise(t_lexer *lexer, char *command)
{
	if (*command == '$' && *(command + 1) == '\0'
		&& !lexer->squote && !lexer->dquote)
		return (fun_add_entry(lexer, command, 1, TOKEN_WORD));
	else if (*command == 39)
		return (fun_single_quote_string(lexer, command));
	else if (*command == 34)
		return (fun_double_quote_string(lexer, command));
	else if (*command == '$' && !lexer->squote)
		return (fun_variable_string(lexer, command));
	else if (*command == '|' && !lexer->squote && !lexer->dquote)
	{
		command = fun_add_entry(lexer, command, 1, TOKEN_PIPE);
		lexer->pipes++;
		return (command);
	}
	else if (*command == '<' || *command == '>')
		return (fun_less_greater(lexer, command));
	else if (fun_check_ifs(*command) && !lexer->squote && !lexer->dquote)
		return (fun_add_entry(lexer, command, 1, TOKEN_IFS));
	return (fun_word_string(lexer, command));
}

/*
 * Fill a linked list with each individual distinct part of the command.
 * $IFS is usually space, tab, new line. Not sure if I should drag it from envp.
 * Params: *lexer, *command
 */
static void	fun_fill_dictionary(t_lexer *lexer, char *command)
{
	while (*command)
	{
		command = fun_charwise(lexer, command);
	}
}

/**
 * Main parsing function
 * Initializes lexer, fills dictionary, and processes tokens
 */
int	parse_line(t_lexer *lexer, char *command, t_mini *mini)
{
	fun_lex_struct_init(lexer);
	fun_fill_dictionary(lexer, command);
	if (init_commands(mini, lexer->pipes + 1) == ERROR)
		return (ERROR);
	if (ms_process_tokens(mini, lexer) == ERROR)
		return (ERROR);
	return (SUCCESS);
}
