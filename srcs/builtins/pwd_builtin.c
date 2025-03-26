/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd_builtin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbyrne <mbyrne@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 09:26:34 by mbyrne            #+#    #+#             */
/*   Updated: 2025/03/26 15:56:20 by mbyrne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <linux/limits.h>

/**
 * @brief Implements the pwd (Print Working Directory) builtin command.
 * 
 * Retrieves and prints the current working directory. Handles errors by
 * printing to stderr and returning an error status.
 * 
 * @return int 0 on success, 1 if getcwd() fails
 * 
 * @note Uses PATH_MAX constant from <linux/limits.h> for buffer size
 * @note Prints the working directory followed by a newline
 * @note Error message follows format "minishell: pwd: [system error message]"
 */
int	pwd_builtin(void)
{
	char	cwd[PATH_MAX];

	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		perror("minishell: pwd");
		return (1);
	}
	ft_putendl_fd(cwd, STDOUT_FILENO);
	return (0);
}
