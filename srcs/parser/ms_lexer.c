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

#include "../includes/minishell.h"

/*
 * Handle strings contained within single quotes.
 * Params: *lexer, *command
 * Return: address of the first character after the quoted string ends
 */
static void	*fun_variable_string(t_lexer *lexer, char *command)
{
	int	len;
	char	*string;

	len = 0;
	while (command[len] != '\0' && !fun_check_ifs(command[len]))
		len++;
	string = ft_substr(command, 0, len);
	fun_add_entry(lexer, string, len, TOKEN_VARIABLE);
	lexer->vars++;
	return (command + len);
}

/*
 * Handle strings contained within single quotes.
 * Params: *lexer, *command
 * Return: address of the first character after the quoted string ends
 */
static void	*fun_single_quote_string(t_lexer *lexer, char *command)
{
	int	len;
	char	*string;

	len = 0;
	fun_flag_flipper(&lexer->squote);
	while (command[len] != '\0' && lexer->squote == 1)
	{
		if (command[len] == 39)
			fun_flag_flipper(&lexer->squote);
		len++;
	}
	string = ft_substr(command, 0, len);
	fun_add_entry(lexer, string, len, TOKEN_SINGLE_Q_STRING);
	return (command + len);
}

/*
 * Handle strings contained within double quotes.
 * Params: *lexer, *command
 * Return: address of the first character after the quoted string ends
 */
static void	*fun_double_quote_string(t_lexer *lexer, char *command)
{
	static int	len;
	int	loc;
	char	*string;

	fun_flag_flipper(&lexer->dquote);
	fun_add_entry(lexer, "\"", len, TOKEN_DOUBLE_Q);
	len = 1;
	loc = len;
	while (command[len] != '\0' && lexer->dquote == 1)
	{
		if (command[len] == '$')
		{
			string = ft_substr(command, loc, len - loc);
			fun_add_entry(lexer, string, len - loc, TOKEN_DOUBLE_Q_CONTENT);
			command = fun_variable_string(lexer, command + len);
			loc = len;
		}
		if (command[len] == 34)
			fun_flag_flipper(&lexer->dquote);
		len++;
	}
	string = ft_substr(command, loc, len);
	fun_add_entry(lexer, string, len - loc, TOKEN_DOUBLE_Q_CONTENT);
	return (command + len);
}

/*
 * Handle plain words (commands and such with no prepositions and not contained
 * 	in any quotes)
 * Params: *lexer, *command
 * Returns: current location within command
 */
static void	*fun_word_string(t_lexer *lexer, char *command)
{
	int	len;
	char	*string;

	len = 0;
	while (command[len] != '\0' && !fun_check_ifs(command[len]))
		len++;
	string = ft_substr(command, 0, len);
	fun_add_entry(lexer, string, len, TOKEN_WORD);
	return (command + len);
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
			fun_variable_string(lexer, command);
		else if (*command == '|' && !lexer->squote && !lexer->dquote)
		{
			fun_add_entry(lexer, "|", 1, TOKEN_PIPE);
			lexer->pipes++;
		}
		else if (*command == '<' && *(command + 1) == '<' && !lexer->squote && !lexer->dquote)
		{
			fun_add_entry(lexer, "<<", 2, TOKEN_REDIR_APPEND);
			lexer->red_delim++;
			command++;
		}
		else if (*command == '>' && *(command + 1) == '>' && lexer->squote && !lexer->dquote)
		{
			fun_add_entry(lexer, ">>", 2, TOKEN_HEREDOC);
			lexer->red_append++;
			command++;
		}
		else if (*command == '<' && !lexer->squote && !lexer->dquote)
		{
			fun_add_entry(lexer, "<", 1, TOKEN_REDIR_IN);
			lexer->red_in++;
		}
		else if (*command == '>' && !lexer->squote && !lexer->dquote)
		{
			fun_add_entry(lexer, ">", 1, TOKEN_REDIR_OUT);
			lexer->red_out++;
		}
		else
			fun_word_string(lexer, command);
		command++;
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
