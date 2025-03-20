#include "../../includes/minishell.h"

size_t ft_strspn(const char *s1, const char *s2)
{
    size_t count;

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

char *remove_quotes(const char *str)
{
    int i;
    int j;
    int len;
    char *result;
    int in_quote[2];

    len = ft_strlen(str);
    result = malloc(len + 1);
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