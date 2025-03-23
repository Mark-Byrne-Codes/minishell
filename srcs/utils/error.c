/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbyrne <mbyrne@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 16:15:12 by mbyrne            #+#    #+#             */
/*   Updated: 2025/03/23 14:53:06 by mbyrne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	print_error(t_mini *mini, char *cmd, char *msg, int status)
{
	ft_putstr_fd("minishell: ", 2);
	if (cmd)
	{
		ft_putstr_fd(cmd, 2);
		ft_putstr_fd(": ", 2);
	}
	if (msg)
		ft_putendl_fd(msg, 2);
	else
		perror("");
	mini->exit_status = status;
}

void	file_error(t_mini *mini, t_command *cmd, char *file)
{
	ft_putstr_fd("minishell: ", 2);
	perror(file);
	cmd->error = 1;
	mini->exit_status = 1;
}

void	clean_exit(t_mini *mini)
{
	int	exit_status;

	if (!mini)
		exit(EXIT_FAILURE);
	exit_status = mini->exit_status;
	if (mini->env)
		free_env_list(mini->env);
	if (mini->commands)
		free_commands(mini);
	if (mini->tokens)
		free_tokens(mini);
	if (mini->fd_in > 0 && mini->fd_in != STDIN_FILENO)
		close(mini->fd_in);
	if (mini->fd_out > 0 && mini->fd_out != STDOUT_FILENO)
		close(mini->fd_out);
	if (mini->pipe_read > 0)
		close(mini->pipe_read);
	if (mini->pipe_write > 0)
		close(mini->pipe_write);
	exit(exit_status);
}

void	exit_error(char *msg, int code)
{
	perror(msg);
	exit(code);
}
