/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_exec_utils_2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbyrne <mbyrne@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 13:00:40 by mbyrne            #+#    #+#             */
/*   Updated: 2025/03/24 09:37:46 by mbyrne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	handle_file_open_error(t_command *cmd, char *filename)
{
	ft_putstr_fd("minishell: ", 2);
	perror(filename);
	free(filename);
	cmd->error = 1;
	return (ERROR);
}

int	setup_input_redir_file(t_command *cmd, char *file)
{
	int		fd;
	char	*unquoted_file;

	if (!file)
		return (ERROR);
	unquoted_file = remove_quotes(file);
	if (!unquoted_file)
		return (ERROR);
	fd = open(unquoted_file, O_RDONLY);
	if (fd == -1)
		return (handle_file_open_error(cmd, unquoted_file));
	free(unquoted_file);
	cmd->fd_in = fd;
	cmd->has_input_redir = 1;
	return (SUCCESS);
}

int	setup_output_redir_file(t_command *cmd, char *file, int append)
{
	int		fd;
	int		flags;
	char	*unquoted_file;

	if (!file)
		return (ERROR);
	flags = O_WRONLY | O_CREAT;
	if (append)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	unquoted_file = remove_quotes(file);
	if (!unquoted_file)
		return (ERROR);
	fd = open(unquoted_file, flags, 0644);
	if (fd == -1)
		return (handle_file_open_error(cmd, unquoted_file));
	free(unquoted_file);
	cmd->fd_out = fd;
	cmd->has_output_redir = 1;
	cmd->append = append;
	return (SUCCESS);
}

int	execute_single_command(t_mini *mini, int i)
{
	int	status;

	if (!mini->commands[i].args || !mini->commands[i].args[0])
		return (SUCCESS);
	if (mini->commands[i].is_builtin && mini->num_commands == 1)
		status = execute_builtin(mini, i);
	else
		status = launch_external(mini, i);
	return (status);
}

int init_commands(t_mini *mini, int num_commands)
{
    int i;

    mini->num_commands = num_commands;
    mini->commands = ft_calloc(num_commands, sizeof(t_command));
    if (!mini->commands)
        return (ERROR);
    i = 0;
    while (i < num_commands)
    {
        ft_memset(&mini->commands[i], 0, sizeof(t_command));
        mini->commands[i].fd_in = STDIN_FILENO;
        mini->commands[i].fd_out = STDOUT_FILENO;
        mini->commands[i].pipe_read = -1;
        mini->commands[i].pipe_write = -1;
        mini->commands[i].mini = mini;
        i++;
    }
    return (SUCCESS);
}