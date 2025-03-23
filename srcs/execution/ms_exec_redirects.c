/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_exec_redirects.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbyrne <mbyrne@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 11:16:39 by mbyrne            #+#    #+#             */
/*   Updated: 2025/03/23 14:46:02 by mbyrne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	process_redirection(t_mini *mini, t_command *cmd, t_redirection *r);

/**
 * Handles all redirections for a command
 * Processes each redirection in the command's redirection list
 */
int	handle_redirection(t_mini *mini, t_command *cmd)
{
	t_redirection	*redir;
	int				redir_error;

	redir = cmd->redirections;
	redir_error = 0;
	while (redir && !redir_error)
	{
		if (process_redirection(mini, cmd, redir) == ERROR)
			redir_error = 1;
		redir = redir->next;
	}
	if (redir_error)
	{
		mini->exit_status = 1;
		return (ERROR);
	}
	if (apply_redirections(cmd) == ERROR)
		return (ERROR);
	return (SUCCESS);
}

/**
 * Processes a single redirection
 * Calls the appropriate setup function based on the redirection type
 */
static int	process_redirection(t_mini *mini, t_command *cmd, t_redirection *r)
{
	(void)mini;
	if (r->type == TOKEN_REDIR_IN)
		return (setup_input_redir_file(cmd, r->file));
	else if (r->type == TOKEN_REDIR_OUT)
		return (setup_output_redir_file(cmd, r->file, 0));
	else if (r->type == TOKEN_HEREDOC)
		return (setup_output_redir_file(cmd, r->file, 1));
	else if (r->type == TOKEN_REDIR_APPEND)
		return (setup_heredoc_delim(cmd, r->file));
	return (SUCCESS);
}

/**
 * Applies the redirections to the current process
 * Uses dup2 to redirect standard input/output
 */
int	apply_redirections(t_command *cmd)
{
	if (cmd->has_input_redir && cmd->fd_in != STDIN_FILENO)
	{
		if (dup2(cmd->fd_in, STDIN_FILENO) == -1)
			return (ERROR);
		if (!cmd->is_heredoc)
			close(cmd->fd_in);
	}
	if (cmd->has_output_redir && cmd->fd_out != STDOUT_FILENO)
	{
		if (dup2(cmd->fd_out, STDOUT_FILENO) == -1)
			return (ERROR);
		close(cmd->fd_out);
	}
	return (SUCCESS);
}

/**
 * Restores the original file descriptors
 * Used after command execution to restore the shell's I/O
 */
void	restore_io(int saved_stdin, int saved_stdout)
{
	if (saved_stdin != STDIN_FILENO)
	{
		dup2(saved_stdin, STDIN_FILENO);
		close(saved_stdin);
	}
	if (saved_stdout != STDOUT_FILENO)
	{
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdout);
	}
}
