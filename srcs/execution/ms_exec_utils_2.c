/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_exec_utils_2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbyrne <mbyrne@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 13:00:40 by mbyrne            #+#    #+#             */
/*   Updated: 2025/03/23 15:01:55 by mbyrne           ###   ########.fr       */
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

	if (mini->commands[i].is_builtin && mini->num_commands == 1)
		status = execute_builtin(mini, i);
	else
		status = launch_external(mini, i);
	return (status);
}
