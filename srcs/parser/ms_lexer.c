/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_lexer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elehtone <elehtone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 15:09:57 by elehtone          #+#    #+#             */
/*   Updated: 2025/03/18 15:00:23 by elehtone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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

static int	ms_handle_pipe(t_mini *mini, t_list **temp, int *cmd_idx, int *arg_idx)
{
	(void)mini;
	*cmd_idx += 1;
	*arg_idx = 0;
	*temp = (*temp)->next;
	return (SUCCESS);
}

static int	ms_handle_token_expansion(t_mini *mini, t_command *cmd, 
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
		if (*arg_idx == 0 && token->token == TOKEN_WORD && is_builtin(token->string))
			cmd->is_builtin = 1;
		(*arg_idx)++;
	}
	free(expanded);
	return (SUCCESS);
}

static int	ms_handle_redirection(t_mini *mini, t_command *cmd, t_list **temp)
{
	(void)mini;
	t_token	*data;
	t_token	*file_token;
	char    *old_file;

	data = (t_token *)(*temp)->content;
	if (!(*temp)->next || ((t_token *)(*temp)->next->content)->token != TOKEN_WORD)
		return (ERROR);
	file_token = (*temp)->next->content;
	
	// Save the old redirection file if it exists
	old_file = cmd->redir_file;
	
	cmd->redir_type = data->token;
	cmd->redir_file = ft_strdup(file_token->string);
	if (!cmd->redir_file)
	{
		if (old_file)
			free(old_file);
		return (ERROR);
	}
	
	// Free the old redirection file
	if (old_file)
		free(old_file);
	
	// Set the appropriate redirection flags
	if (data->token == TOKEN_REDIR_IN || data->token == TOKEN_HEREDOC)
		cmd->has_input_redir = 1;
	else if (data->token == TOKEN_REDIR_OUT || data->token == TOKEN_REDIR_APPEND)
		cmd->has_output_redir = 1;
	
	if (data->token == TOKEN_REDIR_APPEND)
		cmd->append = 1;
	else if (data->token == TOKEN_HEREDOC)
		cmd->is_heredoc = 1;
		
	*temp = (*temp)->next->next;
	return (SUCCESS);
}

static int	ms_process_tokens(t_mini *mini, t_lexer *lexer)
{
	t_list	*temp;
	t_token	*data;
	int		cmd_idx;
	int		arg_idx;
	int		last_token_was_redir;

	temp = lexer->dictionary;
	cmd_idx = 0;
	arg_idx = 0;
	last_token_was_redir = 0;
	while (temp)
	{
		data = (t_token *)temp->content;
		
		// Handle pipe tokens
		if (data->token == TOKEN_PIPE)
		{
			if (ms_handle_pipe(mini, &temp, &cmd_idx, &arg_idx) == ERROR)
				return (ERROR);
			last_token_was_redir = 0;
			continue;
		}
		
		// Handle redirection tokens
		if (data->token == TOKEN_REDIR_IN || data->token == TOKEN_REDIR_OUT ||
			data->token == TOKEN_REDIR_APPEND || data->token == TOKEN_HEREDOC)
		{
			if (ms_handle_redirection(mini, &mini->commands[cmd_idx], &temp) == ERROR)
				return (ERROR);
			last_token_was_redir = 1;
			continue;
		}
		
		// Handle word tokens
		if (data->token == TOKEN_WORD || data->token == TOKEN_VARIABLE)
		{
			// Skip if this is the filename after a redirection
			if (last_token_was_redir)
			{
				last_token_was_redir = 0;
				temp = temp->next;
				continue;
			}
			
			if (ms_handle_token_expansion(mini, &mini->commands[cmd_idx], 
					data, &arg_idx) == ERROR)
				return (ERROR);
		}
		else if (data->token == TOKEN_IFS)
		{
			temp = temp->next;
			continue;
		}
		
		last_token_was_redir = 0;
		temp = temp->next;
	}
	return (SUCCESS);
}

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
