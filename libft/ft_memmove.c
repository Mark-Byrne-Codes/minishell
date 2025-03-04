/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbyrne <mbyrne@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 15:33:40 by mbyrne            #+#    #+#             */
/*   Updated: 2024/11/12 15:26:05 by mbyrne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
/* This function copies memory from source to destination,
 handling overlap. */
void	*ft_memmove(void *dst, const void *src, size_t n)
{
	unsigned char		*temp1;
	const unsigned char	*temp2;

	temp1 = dst;
	temp2 = (unsigned char *)src;
	if (dst < src)
		return (ft_memcpy(dst, src, n));
	if (dst > src)
		while (n--)
			temp1[n] = temp2[n];
	return (dst);
}
