/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbyrne <mbyrne@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 09:55:08 by mbyrne            #+#    #+#             */
/*   Updated: 2024/11/14 15:38:35 by mbyrne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
/* This function splits a string into an array of substrings
 based on a delimiter. */
static size_t	ft_countword(char const *s, char c)
{
	size_t	count;

	if (!*s)
		return (0);
	count = 0;
	while (*s)
	{
		while (*s == c)
			s++;
		if (*s)
			count++;
		while (*s != c && *s)
			s++;
	}
	return (count);
}

static void	free_split(char **lst, int i)
{
	while (i > 0)
		free(lst[--i]);
	free(lst);
}

static char	*get_next_word(char const **s, char c)
{
	size_t	word_len;
	char	*word;

	while (**s == c && **s)
		(*s)++;
	if (!**s)
		return (NULL);
	if (ft_strchr(*s, c))
		word_len = ft_strchr(*s, c) - *s;
	else
		word_len = ft_strlen(*s);
	word = ft_substr(*s, 0, word_len);
	*s += word_len;
	return (word);
}

char	**ft_split(char const *s, char c)
{
	char	**lst;
	int		i;

	if (!s)
		return (NULL);
	lst = (char **)malloc((ft_countword(s, c) + 1) * sizeof(char *));
	if (!lst)
		return (NULL);
	i = 0;
	while (*s)
	{
		lst[i] = get_next_word(&s, c);
		if (lst[i] == NULL && *s != '\0')
		{
			free_split(lst, i);
			return (NULL);
		}
		if (lst[i] != NULL)
			i++;
	}
	lst[i] = NULL;
	return (lst);
}
