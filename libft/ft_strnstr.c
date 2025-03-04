/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbyrne <mbyrne@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 15:34:08 by mbyrne            #+#    #+#             */
/*   Updated: 2024/11/11 13:03:14 by mbyrne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
/* This function locates a substring in a string,
 up to a given length. */
char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t		i;
	size_t		length;
	char		*large;
	char		*small;

	if (*little == '\0')
		return ((char *)big);
	if (len == 0)
		return (NULL);
	large = (char *)big;
	small = (char *)little;
	i = 0;
	length = ft_strlen(small);
	while (large[i] != '\0' && (i + length) <= len)
	{
		if (ft_strncmp((large + i), small, length) == 0)
		{
			return (large + i);
		}
		i++;
	}
	return (NULL);
}
