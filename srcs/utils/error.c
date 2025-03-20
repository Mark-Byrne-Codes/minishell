/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbyrne <mbyrne@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 16:15:12 by mbyrne            #+#    #+#             */
/*   Updated: 2025/03/18 16:15:12 by mbyrne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * Prints an error message with the command name
 * Sets the exit status in the mini struct
 */
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

/**
 * Handles file-related errors
 * Prints an appropriate error message and sets the command's error flag
 */
void	file_error(t_mini *mini, t_command *cmd, char *file)
{
	ft_putstr_fd("minishell: ", 2);
	perror(file);
	cmd->error = 1;
	mini->exit_status = 1;
} 