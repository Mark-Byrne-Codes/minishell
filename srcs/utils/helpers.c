/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbyrne <mbyrne@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 13:48:06 by mbyrne            #+#    #+#             */
/*   Updated: 2025/03/26 11:17:29 by mbyrne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
 * Restores the original file descriptors
 * Used after command execution to restore the shell's I/O
 */
void	restore_io(int saved_stdin, int saved_stdout)
{
	if (saved_stdin != STDIN_FILENO)
	{
		dup2(saved_stdin, STDIN_FILENO);
		close(saved_stdin);
	}
	if (saved_stdout != STDOUT_FILENO)
	{
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdout);
	}
}

/**
 * @brief Checks if a character is a quote character
 * 
 * @param c Character to check
 * @return int 0 if not a quote, 1 if single quote, 2 if double quote
 */
int	is_quote_character(unsigned int c)
{
	if (c == 39)
		return (1);
	else if (c == 34)
		return (2);
	return (0);
}
