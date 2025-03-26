/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbyrne <mbyrne@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 09:16:20 by mbyrne            #+#    #+#             */
/*   Updated: 2025/03/26 15:56:08 by mbyrne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * @brief Implements the echo builtin command.
 * 
 * This function replicates the behavior of the standard echo command:
 * - Prints all arguments separated by spaces
 * - Supports the -n flag to suppress the trailing newline
 * - Returns 0 on success
 * 
 * @param args Array of arguments where:
 *             args[0] is "echo"
 *             args[1] may be "-n" (optional flag)
 *             args[2..n] are the strings to be printed
 * @return int Always returns 0 (success) as echo doesn't have failure cases
 *             in this implementation
 */
int	echo_builtin(char **args)
{
	int	newline;
	int	i;

	newline = 1;
	i = 1;
	if (args[i] && ft_strcmp(args[i], "-n") == 0)
	{
		newline = 0;
		i++;
	}
	while (args[i])
	{
		ft_putstr_fd(args[i], STDOUT_FILENO);
		if (args[i + 1])
			write(STDOUT_FILENO, " ", 1);
		i++;
	}
	if (newline)
		write(STDOUT_FILENO, "\n", 1);
	return (0);
}
