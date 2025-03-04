/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 11:31:26 by mbyrne            #+#    #+#             */
/*   Updated: 2024/12/20 11:47:44 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*Parses the format specifier and calls the corresponding print function.
 Returns the total number of characters printed or -1 on error. */
int	parse_format(const char **format, va_list args)
{
	int	count;

	count = 0;
	if (**format == 'c')
		count += ft_printchar(va_arg(args, int));
	else if (**format == 's')
		count += ft_printstr(va_arg(args, char *));
	else if (**format == 'd' || **format == 'i')
		count += ft_printnbr(va_arg(args, int));
	else if (**format == 'u')
		count += ft_printnbru(va_arg(args, unsigned int));
	else if (**format == 'x')
		count += ft_print_hex(va_arg(args, unsigned int), 0);
	else if (**format == 'X')
		count += ft_print_hex(va_arg(args, unsigned int), 1);
	else if (**format == 'p')
		count += ft_print_pointer(va_arg(args, void *));
	else if (**format == '%')
		count += ft_printchar('%');
	else
	{
		ft_printstr("Error: Invalid format specifier\n");
		return (-1);
	}
	return (count);
}

/*Takes a format string and a variable number of arguments.
 Returns the total number of characters printed, or -1 on error.*/
int	ft_printf(const char *format, ...)
{
	int		count;
	va_list	args;
	int		error;

	va_start(args, format);
	count = 0;
	while (*format)
	{
		if (*format == '%')
		{
			format++;
			error = parse_format(&format, args);
			if (error == -1)
			{
				va_end(args);
				return (-1);
			}
			count += error;
		}
		else
			count += ft_printchar(*format);
		format++;
	}
	va_end(args);
	return (count);
}
