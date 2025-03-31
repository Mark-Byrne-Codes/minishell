/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_processor.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbyrne <mbyrne@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 15:30:12 by mbyrne            #+#    #+#             */
/*   Updated: 2025/03/31 14:26:54 by mbyrne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * @brief Adds an argument to a command's argument array
 * 
 * @param cmd Command structure to modify
 * @param arg Argument string to add
 * @param idx Current argument count
 * @return int SUCCESS on success, ERROR on allocation failure
 * 
 * @note Dynamically expands the argument array and handles:
 * - Memory allocation for new array
 * - String duplication
 * - Proper cleanup of old array
 */
int	add_argument(t_command *cmd, char *arg, int idx)
{
	char	**new_args;
	char	**old_args;
	int		i;

	new_args = malloc(sizeof(char *) * (idx + 2));
	if (!new_args)
		return (ERROR);
	i = 0;
	while (i < idx)
	{
		new_args[i] = cmd->args[i];
		i++;
	}
	new_args[idx] = ft_strdup(arg);
	if (!new_args[idx])
	{
		free(new_args);
		return (ERROR);
	}
	new_args[idx + 1] = NULL;
	old_args = cmd->args;
	cmd->args = new_args;
	if (idx > 0)
		free(old_args);
	return (SUCCESS);
}

/**
 * @brief Processes word and variable tokens
 * 
 * @param mini Minishell context
 * @param cmd Command structure to modify
 * @param data Token being processed
 * @param idx Pointer to current argument index
 * @return int SUCCESS on success, ERROR on failure
 * 
 * @note Handles both regular words and variables by:
 * 1. Expanding variables if needed
 * 2. Adding to command arguments
 * 3. Checking for builtin commands
 */
int	handle_word_var(t_mini *mini, t_command *cmd, t_token *data, int *idx)
{
	char	*exp;
	int		result;

	exp = expand_token(mini, data);
	if (!exp)
		return (ERROR);
	result = SUCCESS;
	if (exp[0] || *idx > 0)
	{
		if (add_argument(cmd, exp, *idx) == ERROR)
			result = ERROR;
		else
		{
			if (*idx == 0 && data->token == TOKEN_WORD && is_builtin(exp))
				cmd->is_builtin = 1;
			(*idx)++;
		}
	}
	free(exp);
	return (result);
}

/**
 * @brief Processes single-quoted string tokens
 * 
 * @param cmd Command structure to modify
 * @param data Token being processed
 * @param idx Pointer to current argument index
 * @return int SUCCESS on success, ERROR on failure
 * 
 * @note Handles single-quoted strings by:
 * 1. Preserving literal content
 * 2. Adding to command arguments
 * 3. Checking for builtin commands
 */
int	handle_single_quote(t_command *cmd, t_token *data, int *idx)
{
	char	*content;

	content = ft_strdup(data->string);
	if (!content)
		return (ERROR);
	if (add_argument(cmd, content, *idx) == ERROR)
	{
		free(content);
		return (ERROR);
	}
	if (*idx == 0)
		cmd->is_builtin = is_builtin(content);
	(*idx)++;
	free(content);
	return (SUCCESS);
}

/**
 * @brief Concatenates two adjacent string tokens
 * 
 * @param d1 First token data
 * @param d2 Second token data
 * @param t1 First token list node
 * @param t2 Second token list node
 * @return int SUCCESS on success, ERROR on failure
 * 
 * @note Combines string content and maintains token list integrity
 * Handles memory allocation and cleanup of merged tokens
 */
static int	concat_tokens(t_token *d1, t_token *d2, t_list *t1, t_list *t2)
{
	char	*new;

	new = ft_strjoin(d1->string, d2->string);
	if (!new)
		return (ERROR);
	free(d1->string);
	d1->string = new;
	t1->next = t2->next;
	free(d2->string);
	free(d2);
	free(t2);
	return (SUCCESS);
}

/**
 * @brief Concatenates adjacent string tokens in lexer dictionary
 * 
 * @param lexer Lexer containing token dictionary
 * @return int SUCCESS on success, ERROR on failure
 * 
 * @note Scans token list and merges adjacent:
 * - Word tokens
 * - Single-quoted strings
 * Preserves other token types unchanged
 */
int	concat_adjacent_strings(t_lexer *lexer)
{
	t_list	*tmp;
	t_token	*data;
	t_token	*nx;

	tmp = lexer->dictionary;
	while (tmp && tmp->next)
	{
		data = (t_token *)tmp->content;
		nx = (t_token *)tmp->next->content;
		if ((data->token == TOKEN_WORD || data->token == TOKEN_SINGLE_Q_STRING)
			&& (nx->token == TOKEN_WORD || nx->token == TOKEN_SINGLE_Q_STRING))
		{
			if (concat_tokens(data, nx, tmp, tmp->next) == ERROR)
				return (ERROR);
		}
		else
			tmp = tmp->next;
	}
	return (SUCCESS);
}
