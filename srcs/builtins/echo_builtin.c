/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbyrne <mbyrne@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 09:16:20 by mbyrne            #+#    #+#             */
/*   Updated: 2025/03/27 11:26:29 by mbyrne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * @brief Checks if an argument is a valid '-n' option
 * 
 * Validates that the argument consists solely of '-'
 * followed by one or more 'n' characters.
 * 
 * @param arg The argument string to check
 * @return int 1 if valid -n option, 0 otherwise
 * 
 */
static int	is_valid_n_option(char *arg)
{
	size_t	i;

	if (!arg || arg[0] != '-')
		return (0);
	i = 1;
	if (!arg[i])
		return (0);
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

/**
 * @brief Implements the echo builtin command
 * 
 * Replicates standard echo behavior with support for:
 * - Printing all arguments separated by spaces
 * - -n option to suppress trailing newline
 * - Multiple consecutive -n options (e.g., -nnnn)
 * 
 * @param args Argument array where:
 *             args[0] = "echo" (command name)
 *             args[1] = optional "-n" flag(s)
 *             args[2..N] = strings to print
 * @return int Always returns 0 (success)
 * 
 * @note Behavior matches POSIX echo specifications:
 * - Multiple -n flags are treated as single -n
 * - Arguments are printed exactly as given
 * - No interpretation of escape sequences
 * - Trailing newline omitted only if -n option present
 * 
 */
int	echo_builtin(char **args)
{
	int	newline;
	int	i;

	newline = 1;
	i = 1;
	while (args[i] && is_valid_n_option(args[i]))
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
