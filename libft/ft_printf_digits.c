/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_digits.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 11:49:29 by mbyrne            #+#    #+#             */
/*   Updated: 2024/12/20 11:47:36 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// Function to print an integer
int	ft_printnbr(int n)
{
	char			c;
	int				count;
	unsigned int	num;

	count = 0;
	if (n == INT_MIN)
	{
		write(1, "-2147483648", 11);
		return (11);
	}
	if (n < 0)
	{
		write(1, "-", 1);
		count++;
		num = (unsigned int)(-n);
	}
	else
	{
		num = (unsigned int)n;
	}
	if (num >= 10)
		count += ft_printnbr(num / 10);
	c = (char)(num % 10 + '0');
	write(1, &c, 1);
	return (count + 1);
}

// Function to print an unsigned integer
int	ft_printnbru(unsigned int n)
{
	int		count;
	char	c;

	count = 0;
	if (n >= 10)
		count += ft_printnbru(n / 10);
	c = (char)(n % 10 + '0');
	write(1, &c, 1);
	return (count + 1);
}

// Function to print a number in hexadecimal format
int	ft_print_hex(unsigned long n, int upper)
{
	int		count;
	char	c;

	count = 0;
	if (n >= 16)
		count += ft_print_hex(n / 16, upper);
	if (n % 16 < 10)
		c = "0123456789"[n % 16];
	else
	{
		if (upper)
			c = "ABCDEF"[n % 16 - 10];
		else
			c = "abcdef"[n % 16 - 10];
	}
	write(1, &c, 1);
	return (count + 1);
}

// Function to print a pointer
int	ft_print_pointer(void *ptr)
{
	int	count;

	if (ptr == NULL)
	{
		write(1, "(nil)", 5);
		return (5);
	}
	count = 0;
	write(1, "0x", 2);
	count += 2;
	count += ft_print_hex((unsigned long)ptr, 0);
	return (count);
}
