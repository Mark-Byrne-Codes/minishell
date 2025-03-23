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

static void	handle_invalid_arg(t_mini *mini, char *arg)
{
	ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
	mini->exit_status = 2;
	clean_exit(mini);
}

static int	handle_sign_and_number(t_mini *mini, char **args, int arg_count)
{
	char		*num_str;
	long long	exit_code;
	int			overflow;

	num_str = ft_strjoin(args[1], args[2]);
	if (!num_str)
		return (ERROR);
	exit_code = convert_to_number(num_str, &overflow);
	free(num_str);
	if (overflow)
		handle_invalid_arg(mini, args[2]);
	if (arg_count > 3)
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", STDERR_FILENO);
		mini->exit_status = 1;
		return (1);
	}
	mini->exit_status = (unsigned char)exit_code;
	clean_exit(mini);
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
		clean_exit(mini);
	if (arg_count >= 3 && (ft_strcmp(args[1], "+") == 0
			|| ft_strcmp(args[1], "-") == 0) && is_numeric(args[2]))
		return (handle_sign_and_number(mini, args, arg_count));
	if (!is_numeric(args[1]))
		handle_invalid_arg(mini, args[1]);
	exit_code = convert_to_number(args[1], &overflow);
	if (overflow)
		handle_invalid_arg(mini, args[1]);
	if (arg_count > 2)
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", STDERR_FILENO);
		mini->exit_status = 1;
		return (1);
	}
	mini->exit_status = (unsigned char)exit_code;
	return (clean_exit(mini), 0);
}
