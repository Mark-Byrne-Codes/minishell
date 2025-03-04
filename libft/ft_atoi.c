/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbyrne <mbyrne@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 15:33:12 by mbyrne            #+#    #+#             */
/*   Updated: 2024/11/13 15:27:03 by mbyrne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
/* This function converts a string to an integer. */
int	ft_atoi(const char *str)
{
	int		letter;
	int		sign;
	long	result;

	sign = 1;
	letter = 0;
	result = 0;
	while ((*str >= 9 && *str <= 13) || *str == 32)
		str++;
	if (*str == '+' || *str == '-')
	{
		if (*str == '-')
			sign *= -1;
		str++;
	}
	while (str[letter] >= '0' && str[letter] <= '9')
	{
		result = result * 10 + (str[letter] - '0');
		if (result < 0 && sign == 1)
			return ((int)LONG_MAX);
		if (result < 0 && sign == -1)
			return ((int)LONG_MIN);
		letter++;
	}
	return ((int)result * sign);
}
