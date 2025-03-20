/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbyrne <mbyrne@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 16:20:12 by mbyrne            #+#    #+#             */
/*   Updated: 2025/03/18 16:20:12 by mbyrne           ###   ########.fr       */
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

/**
 * Checks if a string contains only digits
 * Returns 1 if true, 0 if false
 */
int	ft_is_numeric(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

/**
 * Trims whitespace from the beginning and end of a string
 * Returns a newly allocated string
 */
char	*ft_strtrim_whitespace(char *str)
{
	return (ft_strtrim(str, " \t\n\v\f\r"));
} 

char *ft_str_join(char *s1, char *s2)
{
    char *result;
    size_t len1;
    size_t len2;

    len1 = 0;
    len2 = 0;
    if (s1)
        len1 = ft_strlen(s1);
    if (s2)
        len2 = ft_strlen(s2);
    result = malloc(len1 + len2 + 1);
    if (!result)
        return NULL;
    if (s1)
        ft_strcpy(result, s1);
    else
        result[0] = '\0';
    if (s2)
        ft_strcat(result, s2);
    return result;
}

char	*ft_strjoinchar(const char *s1, char c)
{
	char	*result;
	size_t	len;
	size_t	i;

	if (!s1)
		return (NULL);
	len = ft_strlen(s1);
	result = (char *)malloc(sizeof(char) * (len + 2));
	if (!result)
		return (NULL);
	i = 0;
	while (s1[i])
	{
		result[i] = s1[i];
		i++;
	}
	result[i] = c;
	result[i + 1] = '\0';
	return (result);
}