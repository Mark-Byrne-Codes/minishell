/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbyrne <mbyrne@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 11:29:15 by mbyrne            #+#    #+#             */
/*   Updated: 2025/03/23 14:49:19 by mbyrne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	free_string_array(char **array)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while (array[i] != NULL)
	{
		free(array[i]);
		i++;
	}
	free(array);
}

void	free_tokens(t_mini *mini)
{
	int	i;

	if (mini->tokens)
	{
		i = 0;
		while (i < mini->num_tokens)
		{
			free(mini->tokens[i].string);
			mini->tokens[i].string = NULL;
			i++;
		}
		free(mini->tokens);
		mini->tokens = NULL;
		mini->num_tokens = 0;
	}
}

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

void	free_commands(t_mini *mini)
{
	int	i;

	if (!mini->commands)
		return ;
	i = 0;
	while (i < mini->num_commands)
	{
		if (mini->commands[i].args)
			free_string_array(mini->commands[i].args);
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

void	free_token(t_token *token)
{
	if (!token)
		return ;
	free(token->string);
	free(token);
}
