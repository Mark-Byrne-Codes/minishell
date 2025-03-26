/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_parser.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbyrne <mbyrne@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 12:33:39 by mbyrne            #+#    #+#             */
/*   Updated: 2025/03/26 12:19:56 by mbyrne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * @brief Creates a new redirection node
 * 
 * @param type Token containing redirection type (>, >>, <, <<)
 * @param file Token containing the target filename/delimiter
 * @return t_redirection* New redirection node, or NULL on failure
 * 
 * @note Allocates memory for both the node and file string.
 * Caller is responsible for freeing the structure using free_redirections().
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
 * @brief Adds a redirection node to a command's redirection list
 * 
 * @param cmd Command structure to modify
 * @param new_redir Redirection node to add
 * @return int SUCCESS on success, ERROR on failure
 * 
 * @note Maintains the redirection list in order of appearance.
 * Handles both the first redirection and subsequent additions.
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
 * @brief Processes redirection tokens and builds the redirection list
 * 
 * @param mini Minishell context (unused in current implementation)
 * @param cmd Command structure to store redirections
 * @param temp Pointer to current position in token list
 * @return int SUCCESS on success, ERROR on failure
 * 
 * @note Handles all redirection types (>, >>, <, <<).
 * Skips intervening IFS (whitespace) tokens.
 * Advances the token list pointer to the next non-redirection token.
 */
static int	handle_redirect_parse(t_mini *mini, t_command *cmd, t_list **temp)
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
 * @brief Processes a single token based on its type
 * 
 * @param mini Pointer to minishell structure
 * @param token Current token being processed
 * @param temp Pointer to current token position
 * @param idx Array containing command and argument indices
 * @return int Returns SUCCESS on success, ERROR on failure
 * 
 * @note Routes tokens to appropriate handlers:
 * - Pipes update command counters
 * - Redirections are processed separately
 * - Words/variables are expanded
 * - Single quotes are handled literally
 * - IFS tokens are skipped
 */
static int	process_token_parse(t_mini *mini, t_token *token,
						t_list **temp, int *idx)
{
	t_command	*cmd;

	cmd = &mini->commands[idx[0]];
	if (token->token == TOKEN_PIPE)
	{
		idx[0]++;
		idx[1] = 0;
		*temp = (*temp)->next;
		return (SUCCESS);
	}
	if (token->token >= TOKEN_REDIR_IN && token->token <= TOKEN_HEREDOC)
		return (handle_redirect_parse(mini, cmd, temp));
	if (token->token == TOKEN_WORD || token->token == TOKEN_VARIABLE)
		return (handle_word_var(mini, cmd, token, &idx[1]));
	if (token->token == TOKEN_SINGLE_Q_STRING)
		return (handle_single_quote(cmd, token, &idx[1]));
	if (token->token == TOKEN_IFS)
		*temp = (*temp)->next;
	return (SUCCESS);
}

/**
 * @brief Main token processing function for the parser
 * 
 * @param mini Pointer to minishell structure
 * @param lexer Pointer to lexer structure containing tokens
 * @return int Returns SUCCESS on success, ERROR on failure
 * 
 * @note Performs the following operations:
 * 1. Concatenates adjacent string tokens
 * 2. Initializes command and argument counters
 * 3. Processes each token in sequence
 * 4. Handles error conditions
 */
int	process_tokens(t_mini *mini, t_lexer *lexer)
{
	t_list	*temp;
	t_token	*token;
	int		idx[2];

	if (concat_adjacent_strings(lexer) == ERROR)
		return (ERROR);
	temp = lexer->dictionary;
	idx[0] = 0;
	idx[1] = 0;
	while (temp)
	{
		token = (t_token *)temp->content;
		if (process_token_parse(mini, token, &temp, idx) == ERROR)
			return (ERROR);
		if (token->token != TOKEN_PIPE && token->token != TOKEN_IFS
			&& !(token->token >= TOKEN_REDIR_IN
				&& token->token <= TOKEN_HEREDOC))
			temp = temp->next;
	}
	return (SUCCESS);
}
