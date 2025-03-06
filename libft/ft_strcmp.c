/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbyrne <mbyrne@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 09:11:42 by mbyrne            #+#    #+#             */
/*   Updated: 2025/03/06 09:12:20 by mbyrne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
/* This function compares two strings up to n characters. */
int	ft_strcmp(const char *s1, const char *s2)
{
	unsigned int	z;

	z = 0;
	while ((s1[z] != '\0' || s2[z] != '\0'))
	{
		if (s1[z] != s2[z])
			return ((unsigned char)s1[z] - (unsigned char)s2[z]);
		z++;
	}
	return (0);
}