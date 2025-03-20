/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbyrne <mbyrne@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 17:30:12 by mbyrne            #+#    #+#             */
/*   Updated: 2025/03/18 17:30:12 by mbyrne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * Creates a new redirection node
 * Returns the node or NULL on failure
 */
static t_redirection	*create_redirection(t_token *type, t_token *file)
{
	t_redirection	*new_redir;

	new_redir = malloc(sizeof(t_redirection));
	if (!new_redir)
		return (NULL);
	new_redir->type = type->token;
	new_redir->file = ft_strdup(file->string);
	new_redir->next = NULL;
	if (!new_redir->file)
	{
		free(new_redir);
		return (NULL);
	}
	return (new_redir);
}

/**
 * Adds a redirection to a command
 * Returns SUCCESS or ERROR
 */
static int	add_redirection(t_command *cmd, t_redirection *new_redir)
{
	t_redirection	*redir_list;

	if (!new_redir)
		return (ERROR);
	if (!cmd->redirections)
		cmd->redirections = new_redir;
	else
	{
		redir_list = cmd->redirections;
		while (redir_list->next)
			redir_list = redir_list->next;
		redir_list->next = new_redir;
	}
	return (SUCCESS);
}

/**
 * Handles redirection tokens
 * Creates and adds redirection nodes to the command
 */
int	ms_handle_redirection(t_mini *mini, t_command *cmd, t_list **temp)
{
	t_token			*type_token;
	t_token			*file_token;
	t_redirection	*new_redir;

	(void)mini;
	type_token = (t_token *)(*temp)->content;
	if (!(*temp)->next)
		return (ERROR);
	*temp = (*temp)->next;
	while (*temp && ((t_token *)(*temp)->content)->token == TOKEN_IFS)
	{
		if (!(*temp)->next)
			return (ERROR);
		*temp = (*temp)->next;
	}
	file_token = (t_token *)(*temp)->content;
	new_redir = create_redirection(type_token, file_token);
	if (add_redirection(cmd, new_redir) == ERROR)
		return (ERROR);
	*temp = (*temp)->next;
	return (SUCCESS);
}

/**
 * Processes tokens from the lexer
 * Builds command structures based on tokens
 */
int	ms_process_tokens(t_mini *mini, t_lexer *lexer)
{
	t_list	*temp;
	t_token	*data;
	int		cmd_idx;
	int		arg_idx;

	temp = lexer->dictionary;
	cmd_idx = 0;
	arg_idx = 0;
	while (temp)
	{
		data = (t_token *)temp->content;
		if (data->token == TOKEN_PIPE)
		{
			if (ms_handle_pipe(mini, &temp, &cmd_idx, &arg_idx) == ERROR)
				return (ERROR);
			continue;
		}
		if (data->token >= TOKEN_REDIR_IN && data->token <= TOKEN_HEREDOC)
		{
			if (ms_handle_redirection(mini, &mini->commands[cmd_idx], &temp) == ERROR)
				return (ERROR);
			continue;
		}
		if (data->token == TOKEN_WORD || data->token == TOKEN_VARIABLE)
		{
			if (ms_handle_token_expansion(mini, &mini->commands[cmd_idx], 
					data, &arg_idx) == ERROR)
				return (ERROR);
		}
		else if (data->token == TOKEN_IFS)
		{
			temp = temp->next;
			continue;
		}
		temp = temp->next;
	}
	return (SUCCESS);
}

