/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_exec_heredoc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbyrne <mbyrne@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 15:30:12 by mbyrne            #+#    #+#             */
/*   Updated: 2025/03/23 15:00:46 by mbyrne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	handle_heredoc_input(t_command *cmd, char *delim, int pipe_fd[2]);
static int	process_heredoc_line(t_command *cmd, char *line, int pipe_fd[2]);

int	setup_heredoc_delim(t_command *cmd, char *delim)
{
	int		pipe_fd[2];
	char	*unquoted_delimiter;

	if (pipe(pipe_fd) == -1)
	{
		cmd->error = 1;
		return (ERROR);
	}
	unquoted_delimiter = remove_quotes(delim);
	if (!unquoted_delimiter)
	{
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		return (ERROR);
	}
	if (handle_heredoc_input(cmd, unquoted_delimiter, pipe_fd) == ERROR)
		return (ERROR);
	close(pipe_fd[1]);
	cmd->fd_in = pipe_fd[0];
	cmd->has_input_redir = 1;
	cmd->is_heredoc = 1;
	return (SUCCESS);
}

static int	handle_heredoc_input(t_command *cmd, char *delim, int pipe_fd[2])
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			close(pipe_fd[0]);
			close(pipe_fd[1]);
			free(delim);
			return (ERROR);
		}
		if (ft_strcmp(line, delim) == 0)
		{
			free(line);
			free(delim);
			break ;
		}
		if (process_heredoc_line(cmd, line, pipe_fd) == ERROR)
		{
			free(delim);
			return (ERROR);
		}
	}
	return (SUCCESS);
}

static int	process_heredoc_line(t_command *cmd, char *line, int pipe_fd[2])
{
	char	*expanded_line;

	expanded_line = expand_variables(cmd->mini, line, 0);
	free(line);
	if (!expanded_line)
	{
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		return (ERROR);
	}
	write(pipe_fd[1], expanded_line, ft_strlen(expanded_line));
	write(pipe_fd[1], "\n", 1);
	free(expanded_line);
	return (SUCCESS);
}

char	*remove_quotes(const char *str)
{
	int		i;
	int		j;
	char	*result;
	int		in_quote[2];

	result = malloc(ft_strlen(str) + 1);
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	in_quote[0] = 0;
	in_quote[1] = 0;
	while (str[i])
	{
		if (str[i] == '\'' && !in_quote[1])
			in_quote[0] = !in_quote[0];
		else if (str[i] == '"' && !in_quote[0])
			in_quote[1] = !in_quote[1];
		else
			result[j++] = str[i];
		i++;
	}
	result[j] = '\0';
	return (result);
}
