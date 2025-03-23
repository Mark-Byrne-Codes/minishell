/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_lexer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbyrne <mbyrne@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 15:09:57 by elehtone          #+#    #+#             */
/*   Updated: 2025/03/23 14:30:57 by mbyrne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	*fun_less_greater(t_lexer *lex, char *cmd)
{
	if (*cmd == '<' && *(cmd + 1) == '<' && !lex->squote && !lex->dquote)
	{
		cmd = fun_add_entry(lex, cmd, 2, TOKEN_REDIR_APPEND);
		lex->red_delim++;
	}
	else if (*cmd == '>' && *(cmd + 1) == '>' && !lex->squote && !lex->dquote)
	{
		cmd = fun_add_entry(lex, cmd, 2, TOKEN_HEREDOC);
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
	return (cmd);
}

/*
 * Check the current character and act appropriately.
 * Params: *lexer, *command
 * Return: current location in *command
 */
void	*fun_charwise(t_lexer *lexer, char *command)
{
	while (*command)
	{
		if (*command == 39)
			command = fun_single_quote_string(lexer, command);
		else if (*command == 34)
			command = fun_double_quote_string(lexer, command);
		else if (*command == '$' && !lexer->squote)
			command = fun_variable_string(lexer, command);
		else if (*command == '|' && !lexer->squote && !lexer->dquote)
		{
			command = fun_add_entry(lexer, command, 1, TOKEN_PIPE);
			lexer->pipes++;
		}
		else if (*command == '<' || *command == '>')
		{
			command = fun_less_greater(lexer, command);
		}
		else if (fun_check_ifs(*command) && !lexer->squote && !lexer->dquote)
		{
			command = fun_add_entry(lexer, command, 1, TOKEN_IFS);
		}
		else
			command = fun_word_string(lexer, command);
	}
	return (command);
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
 * Handles token expansion
 * Expands variables and adds arguments to the command
 */
int	ms_handle_token_expansion(t_mini *mini, t_command *cmd, 
	t_token *token, int *arg_idx)
{
char	*expanded;

expanded = expand_token(mini, token);
if (!expanded)
	return (ERROR);
if (expanded[0] != '\0')
{
	if (add_argument(cmd, expanded, *arg_idx) == ERROR)
	{
		free(expanded);
		return (ERROR);
	}
	if (*arg_idx == 0 && token->token == TOKEN_WORD && is_builtin(expanded))
		cmd->is_builtin = 1;
	(*arg_idx)++;
}
free(expanded);
return (SUCCESS);
}

/**
 * Handles pipe tokens in the parser
 * Increments command index and resets argument index
 */
int	ms_handle_pipe(t_mini *mini, t_list **temp, int *cmd_idx, int *arg_idx)
{
	(void)mini;
	*cmd_idx += 1;
	*arg_idx = 0;
	*temp = (*temp)->next;
	return (SUCCESS);
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
