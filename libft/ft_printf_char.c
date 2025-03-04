/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_char.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 11:33:51 by mbyrne            #+#    #+#             */
/*   Updated: 2024/12/20 11:47:29 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// Function to print a single character
int	ft_printchar(int c)
{
	write(1, &c, 1);
	return (1);
}

// Function to print a string
int	ft_printstr(char *str)
{
	int	count;

	count = 0;
	if (!str)
		str = "(null)";
	while (*str)
	{
		write(1, str, 1);
		str++;
		count++;
	}
	return (count);
}
