#include "../../includes/minishell.h"



size_t	ft_strspn(const char *s1, const char *s2)
{
	size_t		count;

	count = 0;
	while (*s1)
	{
		if (ft_strchr(s2, *s1))
			count++;
		else
			break;
		s1++;
	}
	return (count);
}

char *ft_strndup(const char *s, size_t n)
{
    size_t len;
	size_t i;
    char *dup;

	len = 0;
    while (s[len] && len < n)
        len++;
    dup = (char *)malloc(len + 1);
    if (!dup)
        return NULL;
    i = 0;
    while (i < len)
    {
        dup[i] = s[i];
        i++;
    }
    dup[len] = '\0';
    return dup;
}

char *ft_strcpy(char *dest, const char *src)
{
    char *original_dest;

    original_dest = dest;
    while (*src)
    {
        *dest = *src;
        dest++;
        src++;
    }
    *dest = '\0';
    return (original_dest);
}

char *ft_strcat(char *dest, const char *src)
{
    char *original_dest;

    original_dest = dest;
    while (*dest)
        dest++;
    while (*src)
    {
        *dest = *src;
        dest++;
        src++;
    }
    *dest = '\0';
    return (original_dest);
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