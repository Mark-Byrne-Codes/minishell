/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbyrne <mbyrne@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 09:52:58 by mbyrne            #+#    #+#             */
/*   Updated: 2025/04/03 14:35:27 by mbyrne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * @brief Concatenates three strings into a new allocated string
 * 
 * @param s1 First string to concatenate
 * @param s2 Second string to concatenate
 * @param s3 Third string to concatenate
 * @return char* Newly allocated concatenated string, or NULL on failure
 * 
 * @note All input strings must be non-NULL
 * @note Uses ft_calloc for memory allocation
 * @note Caller is responsible for freeing the returned string
 */
char	*ft_strjoin3(char const *s1, char const *s2, char const *s3)
{
	char	*result;
	size_t	len1;
	size_t	len2;
	size_t	len3;

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
 * @brief Joins two strings and frees the original strings
 * 
 * @param s1 First string (will be freed)
 * @param s2 Second string (will be freed)
 * @return char* New concatenated string, or NULL on failure
 * 
 * @note If s1 is NULL, returns duplicate of s2
 * @note If s2 is NULL, frees s1 and returns NULL
 * @note Handles memory allocation failures gracefully
 */
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

/**
 * @brief Removes all quote characters from a string
 * 
 * @param str Input string containing quotes
 * @return char* New string with quotes removed, or NULL on failure
 * 
 * @note Handles nested quotes properly
 * @note Preserves content inside quotes
 * @note Allocates new memory for the result
 */
char	*remove_quotes(const char *str)
{
	int		i;
	int		j;
	char	*result;
	int		in_quote[2];

	result = malloc(ft_strlen(str) + 1);
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	in_quote[0] = 0;
	in_quote[1] = 0;
	while (str[i])
	{
		if (str[i] == '\'' && !in_quote[1])
			in_quote[0] = !in_quote[0];
		else if (str[i] == '"' && !in_quote[0])
			in_quote[1] = !in_quote[1];
		else
			result[j++] = str[i];
		i++;
	}
	result[j] = '\0';
	return (result);
}

/**
 * @brief Determines if a character is a quote character
 * 
 * @param c Character to check
 * @return int 1 for single quote, 2 for double quote, 0 otherwise
 * 
 * @note Uses ASCII values (39 for ', 34 for ")
 * @note Helpful for quote parsing and handling
 */
int	is_quote_character(unsigned int c)
{
	if (c == 39)
		return (1);
	else if (c == 34)
		return (2);
	return (0);
}
