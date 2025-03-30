/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbyrne <mbyrne@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 11:16:39 by mbyrne            #+#    #+#             */
/*   Updated: 2025/03/30 15:51:48 by mbyrne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * @brief Sets up input redirection from a specified file
 * 
 * @param cmd Pointer to the command structure
 * @param file Filename for input redirection (may be quoted)
 * @return int SUCCESS on success, ERROR on failure
 * 
 * @note Removes quotes from filename, opens file in read-only mode,
 * and updates command's input file descriptor.
 */
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

/**
 * @brief Sets up output redirection to a specified file
 * 
 * @param cmd Pointer to the command structure
 * @param file Filename for output redirection (may be quoted)
 * @param append Flag indicating append (1) or truncate (0) mode
 * @return int SUCCESS on success, ERROR on failure
 * 
 * @note Handles both regular output (>) and append (>>) redirections.
 * Removes quotes from filename and sets appropriate open flags.
 */
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

/**
 * @brief Processes a single redirection based on its type
 * 
 * @param cmd Pointer to the command structure
 * @param r Pointer to the redirection structure
 * @return int SUCCESS on success, ERROR on failure
 * 
 * @note Routes to appropriate setup function based on redirection type:
 * - Input redirection (<)
 * - Output redirection (>)
 * - Append redirection (>>)
 * - Heredoc (<<)
 */
static int	process_redirection(t_command *cmd, t_redirection *r)
{
	if (r->type == TOKEN_REDIR_IN)
		return (setup_input_redir_file(cmd, r->file));
	else if (r->type == TOKEN_REDIR_OUT)
		return (setup_output_redir_file(cmd, r->file, 0));
	else if (r->type == TOKEN_REDIR_APPEND)
		return (setup_output_redir_file(cmd, r->file, 1));
	return (SUCCESS);
}

/**
 * @brief Handles all redirections for a command
 * 
 * @param mini Main shell structure
 * @param cmd Current command being processed
 * @return int SUCCESS if all redirections applied, ERROR on failure
 * 
 * @note Skips pre-processed heredocs
 * @warning Sets exit status on error
 */
int	handle_redirection(t_mini *mini, t_command *cmd)
{
	t_redirection	*redir;
	int				status;

	status = SUCCESS;
	redir = cmd->redirections;
	while (redir && status == SUCCESS)
	{
		if (!(redir->type == TOKEN_HEREDOC && redir->processed))
			status = process_redirection(cmd, redir);
		redir = redir->next;
	}
	if (status == ERROR)
	{
		mini->exit_status = 1;
		cmd->exit_status = 1;
		return (ERROR);
	}
	return (apply_redirections(cmd));
}

/**
 * @brief Applies the configured redirections to the current process
 * 
 * @param cmd Pointer to the command structure
 * @return int SUCCESS on success, ERROR on failure
 * 
 * @note Uses dup2 to redirect standard input/output to the appropriate files.
 * Handles special case for heredocs (keeps input fd open).
 * Closes original file descriptors after successful redirection.
 */
int	apply_redirections(t_command *cmd)
{
	if (cmd->has_input_redir && cmd->fd_in != STDIN_FILENO)
	{
		if (dup2(cmd->fd_in, STDIN_FILENO) == -1)
		{
			cmd->error = 1;
			cmd->exit_status = 1;
			return (ERROR);
		}
		if (!cmd->is_heredoc)
			close(cmd->fd_in);
	}
	if (cmd->has_output_redir && cmd->fd_out != STDOUT_FILENO)
	{
		if (dup2(cmd->fd_out, STDOUT_FILENO) == -1)
		{
			cmd->error = 1;
			cmd->exit_status = 1;
			return (ERROR);
		}
		close(cmd->fd_out);
	}
	return (SUCCESS);
}
