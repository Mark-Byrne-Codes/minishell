/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbyrne <mbyrne@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 15:33:31 by mbyrne            #+#    #+#             */
/*   Updated: 2024/11/11 12:36:32 by mbyrne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
/* This function locates the first occurrence
 of a byte in memory. */
void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char	*alts;
	size_t			i;

	alts = (unsigned char *)s;
	i = 0;
	while (n > i)
	{
		if (alts[i] == (unsigned char)c)
			return (&alts[i]);
		i++;
	}
	return (NULL);
}
