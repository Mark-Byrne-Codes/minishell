/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbyrne <mbyrne@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 08:50:10 by mbyrne            #+#    #+#             */
/*   Updated: 2025/03/23 13:19:41 by mbyrne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*replace_home_with_tilde(char *cwd, const char *home)
{
	size_t	home_len;
	char	*result;

	if (!home || !cwd)
		return (ft_strdup(cwd));
	home_len = ft_strlen(home);
	if (ft_strncmp(cwd, home, home_len) == 0
		&& (cwd[home_len] == '/' || cwd[home_len] == '\0'))
	{
		if (cwd[home_len] == '\0')
			return (ft_strdup("~"));
		result = ft_strjoin("~", cwd + home_len);
		return (result);
	}
	return (ft_strdup(cwd));
}

static char	*get_hostname(void)
{
	char	*hostname;
	char	hostname_buf[256];
	char	*dot_pos;
	char	*clean_hostname;

	hostname = getenv("HOSTNAME");
	if (!hostname)
	{
		if (gethostname(hostname_buf, sizeof(hostname_buf)) == 0)
			hostname = hostname_buf;
		else
			return (ft_strdup("localhost"));
	}
	clean_hostname = ft_strdup(hostname);
	dot_pos = ft_strchr(clean_hostname, '.');
	if (dot_pos)
		*dot_pos = '\0';
	return (clean_hostname);
}

static char	*create_user_prefix(char *user)
{
	char	*temp;
	char	*prefix;
	char	*host;

	if (!user)
		return (NULL);
	host = get_hostname();
	temp = ft_strjoin(user, "@");
	prefix = ft_strjoin(temp, host);
	free(temp);
	free(host);
	temp = ft_strjoin(prefix, ":");
	free(prefix);
	return (temp);
}

static char	*format_prompt(char *prefix, char *path)
{
	char	*prompt;
	char	*temp;

	if (prefix)
	{
		temp = ft_strjoin(prefix, path);
		prompt = ft_strjoin(temp, "$ ");
		free(temp);
	}
	else
		prompt = ft_strdup(path);
	return (prompt);
}

char	*get_prompt(void)
{
	char	cwd[PATH_MAX];
	char	*modified_cwd;
	char	*prompt;
	char	*prefix;

	if (!getcwd(cwd, sizeof(cwd)))
	{
		perror("getcwd");
		return (ft_strdup("minishell> "));
	}
	modified_cwd = replace_home_with_tilde(cwd, getenv("HOME"));
	if (!modified_cwd)
		return (ft_strdup("minishell> "));
	prefix = create_user_prefix(getenv("USER"));
	prompt = format_prompt(prefix, modified_cwd);
	free(prefix);
	free(modified_cwd);
	if (!prompt)
		return (ft_strdup("minishell> "));
	return (prompt);
}
