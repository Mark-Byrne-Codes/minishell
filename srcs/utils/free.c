/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbyrne <mbyrne@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 11:29:15 by mbyrne            #+#    #+#             */
/*   Updated: 2025/03/26 11:17:41 by mbyrne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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

/* Free a single token */
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
