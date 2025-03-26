/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbyrne <mbyrne@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 10:24:18 by mbyrne            #+#    #+#             */
/*   Updated: 2025/03/26 15:57:36 by mbyrne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * @brief Checks if a string represents a valid numeric value.
 * 
 * Validates that the string contains only digits, optional sign (+/-),
 * and optional quotes surrounding the number. At least one digit must be there.
 * 
 * @param str The string to validate
 * @return int 1 if the string is numeric, 0 otherwise
 */
static int	is_numeric(const char *str)
{
	int	i;
	int	has_digits;

	if (!str || !*str)
		return (0);
	i = 0;
	has_digits = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (str[i] == '"' || str[i] == '\'')
		i++;
	while (str[i] && ft_isdigit(str[i]))
	{
		has_digits = 1;
		i++;
	}
	if (str[i] && (str[i] == '"' || str[i] == '\''))
		i++;
	return (str[i] == '\0' && has_digits);
}

/**
 * @brief Converts a string to a long long number with overflow detection.
 * 
 * Handles optional sign and quotes, converts the numeric portion to a number,
 * and detects overflow conditions during conversion.
 * 
 * @param str The string to convert
 * @param overflow Pointer to store overflow status (1 if overflow occurred)
 * @return long long The converted number (undefined if overflow occurred)
 */
static long long	convert_to_number(const char *str, int *overflow)
{
	long long	result;
	int			sign;
	int			i;

	result = 0;
	sign = 1;
	*overflow = 0;
	i = 0;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	if (str[i] == '"' || str[i] == '\'')
		i++;
	while (str[i] && ft_isdigit(str[i]) && !*overflow)
	{
		if ((result > LLONG_MAX / 10) || (sign == 1 && \
			result * 10 > LLONG_MAX - (str[i] - '0')))
			*overflow = 1;
		result = result * 10 + (str[i++] - '0');
	}
	return (result * sign);
}

/**
 * @brief Handles exit command error cases.
 * 
 * Sets appropriate error messages and exit status for different failure cases:
 * 1. Too many arguments
 * 2. Non-numeric argument
 * 
 * @param mini Pointer to minishell structure
 * @param error_type Type of error (1 for too many args, 2 for non-numeric)
 * @param arg The problematic argument (used for error message)
 * @return int Always returns 0 (consistent with shell exit behavior)
 */
static int	handle_exit_error(t_mini *mini, int error_type, char *arg)
{
	if (error_type == 1)
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		mini->exit_status = 1;
	}
	else
	{
		ft_putstr_fd("minishell: exit: numeric argument required\n", 2);
		mini->exit_status = 2;
		mini->should_exit = 1;
	}
	(void)arg;
	return (0);
}

/**
 * @brief Implements the exit builtin command.
 * 
 * Handles shell exit with optional status code. Validates arguments and sets:
 * - Exit status (0 if no argument, 1 for too many args, 2 for invalid number)
 * - should_exit flag to terminate the shell
 * 
 * @param mini Pointer to minishell structure
 * @param args Command arguments (args[0] is "exit", args[1] is optional status)
 * @return int Always returns 0 (exit happens through mini->should_exit)
 */
int	exit_builtin(t_mini *mini, char **args)
{
	int			arg_count;
	long long	exit_code;
	int			overflow;

	arg_count = 0;
	while (args[arg_count])
		arg_count++;
	if (arg_count == 1)
	{
		mini->should_exit = 1;
		return (0);
	}
	if (!is_numeric(args[1]))
		return (handle_exit_error(mini, 2, args[1]));
	if (arg_count > 2)
		return (handle_exit_error(mini, 1, NULL));
	exit_code = convert_to_number(args[1], &overflow);
	if (overflow)
		return (handle_exit_error(mini, 2, args[1]));
	mini->exit_status = (unsigned char)exit_code;
	mini->should_exit = 1;
	return (0);
}
