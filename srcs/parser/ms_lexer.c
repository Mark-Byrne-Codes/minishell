/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_lexer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elehtone <elehtone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 15:09:57 by elehtone          #+#    #+#             */
/*   Updated: 2025/03/11 18:49:57 by elehtone         ###   ########.fr       */
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
 * Fill a linked list with each individual distinct part of the command.
 * $IFS is usually space, tab, new line. Not sure if I should drag it from envp.
 * Params: *lexer, *command
 */
static void	fun_fill_dictionary(t_lexer *lexer, char *command)
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
}

/*
 * Pperform the lexing actions. Count special characters for later. Fill
 * 	dictionary list.
 * Params: *lexer, *str
 * Returns: int (number of unbounded character)
 */
 int parse_line(t_lexer *lexer, char *command)
 {
     fun_lex_struct_init(lexer);
     fun_fill_dictionary(lexer, command);
 
     printf("Pipes: %d, vars: %d, redirects: %d / %d, doubles: %d / %d\n", lexer->pipes, lexer->vars, lexer->red_in, lexer->red_out, lexer->red_delim, lexer->red_append);
 
     t_list *temp = lexer->dictionary;
     while (temp != NULL)
     {
         t_token *data = (t_token *)temp->content; // Cast to t_token *
         if (data != NULL)
         {
             printf("Current content add: %p\n", temp->content);
             printf("Next content add: %p\n", temp->next);
             printf("Current content: %s\n", data->string);
             printf("Current len: %d\n", data->len);
             printf("Current tok type: %d\n", data->token); // Print the enum value
         }
         temp = temp->next;
     }
 
     return (0);
 }
