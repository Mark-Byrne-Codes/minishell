/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbyrne <mbyrne@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 13:48:06 by mbyrne            #+#    #+#             */
/*   Updated: 2025/04/03 14:35:38 by mbyrne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	execute_external_command(t_mini *mini, int cmd_idx, char *cmd_path)
{
	char	**env_array;

	env_array = env_list_to_array(mini->env);
	execve(cmd_path, mini->commands[cmd_idx].args, env_array);
	free_string_array(env_array);
	perror("execve failed");
}

/**
 * @brief Restores standard input/output to original state
 * 
 * @param saved_stdin Saved stdin file descriptor
 * @param saved_stdout Saved stdout file descriptor
 * 
 * @note Only restores if the saved descriptors differ from STDIN/STDOUT
 * @note Closes the saved descriptors after restoration
 * @note Typically used after command execution with redirected I/O
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
