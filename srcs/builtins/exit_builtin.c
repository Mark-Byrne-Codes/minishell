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
