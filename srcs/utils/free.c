/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbyrne <mbyrne@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 11:29:15 by mbyrne            #+#    #+#             */
/*   Updated: 2025/03/27 14:10:24 by mbyrne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * @brief Frees a NULL-terminated array of strings
 * 
 * @param array The array to free (may be NULL)
 * 
 * @note Safely handles NULL input
 * @note Frees each string element before freeing the array itself
 * @note Sets pointers to NULL after freeing
 */
void	free_string_array(char **array)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

/**
 * @brief Frees a single token structure
 * 
 * @param content Void pointer to t_token (for use with ft_lstclear)
 * 
 * @note Handles NULL input safely
 * @note Frees both the token string and the token structure itself
 * @note Compatible with ft_lstclear as a del function
 */
void	free_token(void *content)
{
	t_token	*token;

	token = (t_token *)content;
	if (!token)
		return ;
	if (token->string)
		free(token->string);
	free(token);
}

/**
 * @brief Frees all redirection structures in a command
 * 
 * @param cmd The command containing the redirections to free
 * 
 * @note Iterates through the linked list of redirections
 * @note Frees both the filename strings and the redirection nodes
 * @note Sets the redirections pointer to NULL after freeing
 * @note Handles NULL cmd input safely
 */
void	free_redirections(t_command *cmd)
{
	t_redirection	*current;
	t_redirection	*next;

	current = cmd->redirections;
	while (current)
	{
		next = current->next;
		if (current->file)
			free(current->file);
		free(current);
		current = next;
	}
	cmd->redirections = NULL;
}

/**
 * @brief Frees all command structures and their resources
 * 
 * @param mini The main shell structure containing commands to free
 * 
 * @note Performs complete cleanup of:
 * - Command argument arrays
 * - Redirection structures
 * - Open file descriptors (except STDIN/STDOUT)
 * - Command array itself
 * @note Handles NULL inputs safely
 * @note Sets pointers to NULL after freeing
 */
void	free_commands(t_mini *mini)
{
	int	i;

	if (!mini || !mini->commands)
		return ;
	i = 0;
	while (i < mini->num_commands)
	{
		if (mini->commands[i].args)
		{
			free_string_array(mini->commands[i].args);
			mini->commands[i].args = NULL;
		}
		free_redirections(&mini->commands[i]);
		if (mini->commands[i].fd_in != STDIN_FILENO)
			close(mini->commands[i].fd_in);
		if (mini->commands[i].fd_out != STDOUT_FILENO)
			close(mini->commands[i].fd_out);
		if (mini->commands[i].pipe_read != -1)
			close(mini->commands[i].pipe_read);
		if (mini->commands[i].pipe_write != -1)
			close(mini->commands[i].pipe_write);
		i++;
	}
	free(mini->commands);
	mini->commands = NULL;
}
