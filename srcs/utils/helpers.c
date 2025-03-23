/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbyrne <mbyrne@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 13:48:06 by mbyrne            #+#    #+#             */
/*   Updated: 2025/03/23 14:04:27 by mbyrne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"


/**
 * Joins three strings together
 * Returns a newly allocated string
 */
char *ft_strjoin3(char const *s1, char const *s2, char const *s3)
{
    char    *result;
    size_t  len1;
    size_t  len2;
    size_t  len3;

    if (!s1 || !s2 || !s3)
        return (NULL);
    len1 = ft_strlen(s1);
    len2 = ft_strlen(s2);
    len3 = ft_strlen(s3);
    result = ft_calloc(len1 + len2 + len3 + 1, sizeof(char));
    if (!result)
        return (NULL);
    ft_strlcpy(result, s1, len1 + 1);
    ft_strlcpy(result + len1, s2, len2 + 1);
    ft_strlcpy(result + len1 + len2, s3, len3 + 1);
    return (result);
}

char	*join_and_free(char *s1, char *s2)
{
	char	*result;
	size_t	len1;
	size_t	len2;

	if (!s1)
		return (ft_strdup(s2));
	if (!s2)
    {
        free(s1);
		return (NULL);
    }
	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	result = (char *)malloc(len1 + len2 + 1);
	if (!result)
    {
        free(s1);
        free(s2);
		return (NULL);
    }
	ft_memcpy(result, s1, len1);
	ft_memcpy(result + len1, s2, len2 + 1);
	free(s1);
	free(s2);
	return (result);
}