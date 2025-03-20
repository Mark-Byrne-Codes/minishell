/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbyrne <mbyrne@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 11:29:15 by mbyrne            #+#    #+#             */
/*   Updated: 2025/03/16 15:57:30 by mbyrne           ###   ########.fr       */
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

void clean_exit(t_mini *mini)
{
	if (mini)
	{
		if (mini->env)
			free_env_list(mini->env);
    }
	exit(mini->exit_status);
}

char	*join_and_free(char *s1, char *s2)
{
	char	*result;
	size_t	len1;
	size_t	len2;

	if (!s1)
		return (ft_strdup(s2));
	if (!s2)
    {
        free(s1);
		return (NULL);
    }
	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	result = (char *)malloc(len1 + len2 + 1);
	if (!result)
    {
        free(s1);
        free(s2);
		return (NULL);
    }
	ft_memcpy(result, s1, len1);
	ft_memcpy(result + len1, s2, len2 + 1);
	free(s1);
	free(s2);
	return (result);
}

void exit_error(char *msg, int code)
{
    perror(msg);
    exit(code);
}

void free_tokens(t_mini *mini)
{
    int i;

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

void free_redirections(t_command *cmd)
{
    t_redirection *current;
    t_redirection *next;

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

void free_commands(t_mini *mini)
{
    int i;

    if (!mini->commands)
        return;
    
    i = 0;
    while (i < mini->num_commands)
    {
        if (mini->commands[i].args)
            free_string_array(mini->commands[i].args);
        if (mini->commands[i].redir_file)
            free(mini->commands[i].redir_file);
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

void    free_token(t_token *token)
{
    if (!token)
        return ;
    free(token->string);
    free(token);
}