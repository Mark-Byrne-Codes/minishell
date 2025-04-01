/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbyrne <mbyrne@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 08:50:10 by mbyrne            #+#    #+#             */
/*   Updated: 2025/04/01 10:00:33 by mbyrne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * @brief Replaces home directory path with tilde (~) 
 * in current working directory
 * 
 * @param cwd Current working directory path
 * @param home Home directory path
 * @return char* New string with home replaced by ~, 
 * or duplicate of cwd if no match
 * 
 * @note If home is not found at start of cwd, returns duplicate of cwd
 * Allocates memory for the new string
 */
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

/**
 * @brief Gets the cleaned hostname (without domain)
 * 
 * @return char* Hostname string (allocated)
 * 
 * @note Tries HOSTNAME env var first, then gethostname()
 * Removes everything after first dot if present
 * Returns "localhost" if all methods fail
 */
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

/**
 * @brief Creates user@host: prefix string
 * 
 * @param user Username (from environment)
 * @return char* Formatted prefix string (allocated)
 * 
 * @note Returns NULL if user is NULL
 * Caller must free the returned string
 */
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

/**
 * @brief Formats the complete prompt string
 * 
 * @param prefix User@host: prefix
 * @param path Current working directory
 * @return char* Complete prompt string (allocated)
 * 
 * @note If prefix is NULL, uses just path + $
 * Caller must free the returned string
 */
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

/**
 * @brief Generates the shell prompt string
 * 
 * @return char* Formatted prompt string (allocated)
 * 
 * @note Format: [user@host:path$ ] 
 * Replaces home directory with ~
 * Falls back to "minishell> " on errors
 * Caller must free the returned string
 */
char	*get_prompt(void)
{
	char	cwd[PATH_MAX];
	char	*modified_cwd;
	char	*prompt;
	char	*prefix;

	if (!getcwd(cwd, sizeof(cwd)))
	{
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
