

#include "../../includes/minishell.h"

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

void clean_exit(t_mini *mini)
{
	if (mini)
	{
		if (mini->env)
			free_env_list(mini->env);
    }
	exit(mini->exit_status);
}