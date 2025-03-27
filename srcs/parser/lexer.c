/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbyrne <mbyrne@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 15:09:57 by elehtone          #+#    #+#             */
/*   Updated: 2025/03/27 11:44:45 by mbyrne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * @brief Handles redirection operators (< > << >>)
 * 
 * @param lex Lexer structure
 * @param cmd Current position in command string
 * @return void* Updated position in command string or NULL if not a redirection
 */
static void	*handle_redirection_operators(t_lexer *lex, char *cmd)
{
	if (*cmd == '<' && *(cmd + 1) == '<' && !lex->squote && !lex->dquote)
	{
		cmd = add_token_to_list(lex, cmd, 2, TOKEN_HEREDOC);
		lex->red_delim++;
	}
	else if (*cmd == '>' && *(cmd + 1) == '>' && !lex->squote && !lex->dquote)
	{
		cmd = add_token_to_list(lex, cmd, 2, TOKEN_REDIR_APPEND);
		lex->red_append++;
	}
	else if (*cmd == '<' && !lex->squote && !lex->dquote)
	{
		cmd = add_token_to_list(lex, cmd, 1, TOKEN_REDIR_IN);
		lex->red_in++;
	}
	else if (*cmd == '>' && !lex->squote && !lex->dquote)
	{
		cmd = add_token_to_list(lex, cmd, 1, TOKEN_REDIR_OUT);
		lex->red_out++;
	}
	else
		return (NULL);
	while (*cmd && is_whitespace_character(*cmd))
		cmd++;
	return (cmd);
}

/**
 * @brief Processes a single character and determines its token type
 * 
 * @param lexer Lexer structure
 * @param command Current position in command string
 * @return void* Updated position in command string
 */
static void	*process_character(t_lexer *lexer, char *command)
{
	if (*command == '$' && *(command + 1) == '\0'
		&& !lexer->squote && !lexer->dquote)
		return (add_token_to_list(lexer, command, 1, TOKEN_WORD));
	else if (*command == 39)
		return (handle_single_quoted_string(lexer, command));
	else if (*command == 34)
		return (handle_double_quoted_string(lexer, command));
	else if (*command == '$' && !lexer->squote)
		return (handle_variable_token(lexer, command));
	else if (*command == '|' && !lexer->squote && !lexer->dquote)
	{
		command = add_token_to_list(lexer, command, 1, TOKEN_PIPE);
		lexer->pipes++;
		return (command);
	}
	else if (*command == '<' || *command == '>')
		return (handle_redirection_operators(lexer, command));
	else if (is_whitespace_character(*command) && \
		!lexer->squote && !lexer->dquote)
		return (add_token_to_list(lexer, command, 1, TOKEN_IFS));
	return (handle_word_token(lexer, command));
}

/**
 * @brief Populates the token list by processing each character
 * 
 * @param lexer Lexer structure
 * @param command Command string to tokenize
 */
static void	populate_token_list(t_lexer *lexer, char *command)
{
	while (*command)
	{
		command = process_character(lexer, command);
	}
}

/**
 * @brief Main parsing function that tokenizes and processes a command line
 * 
 * @param lexer Lexer structure to populate
 * @param command Command string to parse
 * @param mini Minishell context
 * @return int SUCCESS on successful parsing, ERROR otherwise
 */
int	parse_line(t_lexer *lexer, char *command, t_mini *mini)
{
	int	ret;

	init_lexer(lexer);
	populate_token_list(lexer, command);
	ret = init_commands(mini, lexer->pipes + 1);
	if (ret == ERROR)
	{
		cleanup_lexer(lexer);
		return (ERROR);
	}
	ret = process_tokens(mini, lexer);
	cleanup_lexer(lexer);
	return (ret);
}
