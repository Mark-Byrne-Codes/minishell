/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbyrne <mbyrne@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 10:24:18 by mbyrne            #+#    #+#             */
/*   Updated: 2025/03/23 13:24:32 by mbyrne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	is_numeric(const char *str)
{
	const char	*start;

	if (!str || !*str)
		return (0);
	if (*str == '+' || *str == '-')
		str++;
	if (*str == '"' || *str == '\'')
		str++;
	if (!*str)
		return (0);
	start = str;
	while (*str)
	{
		if (*str == '"' || *str == '\'')
		{
			if (str != start && *(str + 1) == '\0')
				break ;
			else
				return (0);
		}
		if (!ft_isdigit(*str))
			return (0);
		str++;
	}
	return (1);
}

static int	pro_digit(long long *result, int sign, char digit, int *overflow)
{
	long long	temp_result;

	temp_result = *result * 10 + (digit - '0');
	if ((temp_result > LLONG_MAX / 10) || (temp_result < LLONG_MIN / 10)
		|| (sign == 1 && temp_result > LLONG_MAX - (digit - '0'))
		|| (sign == -1 && temp_result < LLONG_MIN + (digit - '0')))
	{
		*overflow = 1;
		return (1);
	}
	*result = temp_result;
	return (0);
}

static long long	ft_atoll_with_overflow(const char *str, int *overflow)
{
	long long	result;
	int			sign;

	result = 0;
	sign = 1;
	*overflow = 0;
	if (*str == '+' || *str == '-')
	{
		if (*str == '-')
			sign = -1;
		else
			sign = 1;
		str++;
	}
	if (*str == '"' || *str == '\'')
		str++;
	while (ft_isdigit(*str) && !pro_digit(&result, sign, *str, overflow))
		str++;
	if (*str == '"' || *str == '\'')
		str++;
	return (result * sign);
}

static void	handle_invalid_argument(t_mini *mini, char *arg)
{
	ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
	mini->exit_status = 2;
	clean_exit(mini);
}

int	exit_builtin(t_mini *mini, char **args)
{
	int			arg_count;
	int			overflow;
	long long	exit_code;

	arg_count = 0;
	while (args[arg_count])
		arg_count++;
	if (arg_count == 1)
		clean_exit(mini);
	if (!is_numeric(args[1]))
		handle_invalid_argument(mini, args[1]);
	exit_code = ft_atoll_with_overflow(args[1], &overflow);
	if (overflow)
		handle_invalid_argument(mini, args[1]);
	if (arg_count > 2)
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", STDERR_FILENO);
		mini->exit_status = 1;
		return (1);
	}
	mini->exit_status = (unsigned char)exit_code;
	clean_exit(mini);
	return (0);
}
