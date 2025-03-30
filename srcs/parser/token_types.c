/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_types.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbyrne <mbyrne@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 00:27:13 by elehtone          #+#    #+#             */
/*   Updated: 2025/03/30 15:46:47 by mbyrne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*
 * Handle strings contained within single quotes.
 * Params: *lexer, *command
 * Return: address of the first character after the quoted string ends
 */
void	*handle_variable_token(t_lexer *lexer, char *cmd)
{
	int	len;

	len = 0;
	while (cmd[len] != '\0' && !is_whitespace_character(cmd[len])
		&& !is_quote_character(cmd[len]))
		len++;
	cmd = add_token_to_list(lexer, cmd, len, TOKEN_VARIABLE);
	lexer->vars++;
	return (cmd);
}

/*
 * Handle plain words (commands and such with no prepositions and not contained
 * 	in any quotes)
 * Params: *lexer, *command
 * Returns: current location within command
 */
void	*handle_word_token(t_lexer *lexer, char *cmd)
{
	int	i;

	i = 0;
	while (cmd[i] && !is_whitespace_character(cmd[i])
		&& !is_quote_character(cmd[i])
		&& cmd[i] != '$' && cmd[i] != '|'
		&& cmd[i] != '<' && cmd[i] != '>')
		i++;
	if (i > 0)
		cmd = add_token_to_list(lexer, cmd, i, TOKEN_WORD);
	return (cmd);
}

/*
 * Handle strings contained within double quotes.
 * Params: *lexer, *command
 * Return: address of the first character after the quoted string ends
 */
void	*handle_double_quoted_string(t_lexer *lexer, char *cmd)
{
	char	*start;
	int		len;
	char	*content;

	toggle_quote_state(&lexer->dquote);
	start = cmd + 1;
	len = 0;
	while (start[len] != '\0' && start[len] != '"')
		len++;
	content = ft_substr(start, 0, len);
	if (!content)
		return (NULL);
	add_token_to_list(lexer, content, len, TOKEN_WORD);
	free(content);
	if (start[len] == '"')
		toggle_quote_state(&lexer->dquote);
	if (start[len] == '"')
		return (start + len + 1);
	else
		return (start + len);
}

/*
 * Handle strings contained within single quotes.
 * Params: *lexer, *command
 * Return: address of the first character after the quoted string ends
 */
void	*handle_single_quoted_string(t_lexer *lexer, char *cmd)
{
	char	*start;
	int		len;
	char	*content;

	toggle_quote_state(&lexer->squote);
	start = cmd + 1;
	len = 0;
	while (start[len] != '\0' && start[len] != '\'')
		len++;
	content = ft_substr(start, 0, len);
	if (!content)
		return (NULL);
	add_token_to_list(lexer, content, len, TOKEN_SINGLE_Q_STRING);
	free(content);
	if (start[len] == '\'')
		toggle_quote_state(&lexer->squote);
	if (start[len] == '\'')
		return (start + len + 1);
	else
		return (start + len);
}

/**
 * @brief Processes all heredocs before command execution
 * 
 * @param mini Pointer to minishell structure
 * @return int SUCCESS if all heredocs processed, ERROR on failure
 * 
 * @note Sets redir->processed flag for each heredoc
 * @warning Stops processing on first error
 */
int	preprocess_heredocs(t_mini *mini)
{
	t_command		*cmd;
	t_redirection	*redir;
	int				i;

	i = 0;
	while (i < mini->num_commands)
	{
		cmd = &mini->commands[i];
		redir = cmd->redirections;
		while (redir)
		{
			if (redir->type == TOKEN_HEREDOC)
			{
				if (setup_heredoc_delim(cmd, redir->file) == ERROR)
					return (ERROR);
				redir->processed = 1;
			}
			redir = redir->next;
		}
		i++;
	}
	return (SUCCESS);
}
