/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbyrne <mbyrne@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 11:28:22 by mbyrne            #+#    #+#             */
/*   Updated: 2025/03/26 15:58:20 by mbyrne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * @brief Creates an env node from a "NAME=value" string
 * 
 * @param env_str Environment string to parse
 * @return t_env* New node or NULL on failure
 * 
 * @note Splits string at first '=' character
 * Allocates memory for both name and value
 * Caller must free the node using free_env_list()
 */
static t_env	*create_env_node_from_string(char *env_str)
{
	char	*equals;
	char	*name;
	char	*value;
	t_env	*new_node;

	equals = ft_strchr(env_str, '=');
	if (!equals)
		return (NULL);
	name = ft_substr(env_str, 0, equals - env_str);
	if (!name)
		return (NULL);
	value = ft_strdup(equals + 1);
	if (!value)
	{
		free(name);
		return (NULL);
	}
	new_node = create_env_node(name, value);
	free(name);
	free(value);
	return (new_node);
}

/**
 * @brief Counts nodes in environment list
 * 
 * @param head List head pointer
 * @return int Number of nodes
 */
static int	count_nodes(t_env *head)
{
	int	count;

	count = 0;
	while (head)
	{
		count++;
		head = head->next;
	}
	return (count);
}

/**
 * @brief Converts envp array to linked list
 * 
 * @param envp NULL-terminated array of "NAME=value" strings
 * @return t_env* Head of new list or NULL on failure
 * 
 * @note Creates a deep copy of all environment variables
 * On failure, frees any partially created list
 */
t_env	*copy_env(char **envp)
{
	t_env	*head;
	t_env	*new_node;
	int		i;

	head = NULL;
	i = 0;
	while (envp[i])
	{
		new_node = create_env_node_from_string(envp[i]);
		if (!new_node)
			return (free_env_list(head), NULL);
		add_env_node(&head, new_node);
		i++;
	}
	return (head);
}

/**
 * @brief Converts env list to string array
 * 
 * @param head List head pointer
 * @return char** NULL-terminated array of "NAME=value" strings
 * 
 * @note Allocates new memory for the array and strings
 * Returns NULL on allocation failure after freeing partial results
 */
char	**env_list_to_array(t_env *h)
{
	char	**arr;
	int		i;
	int		c;
	char	*tmp;

	c = count_nodes(h);
	arr = ft_calloc(c + 1, sizeof(char *));
	if (!arr || !h)
		return (NULL);
	i = 0;
	while (h)
	{
		if (h->name && h->value)
		{
			tmp = ft_strjoin(h->name, "=");
			arr[i++] = ft_strjoin(tmp, h->value);
			free(tmp);
			if (!arr[i - 1])
				return (free_string_array(arr), NULL);
		}
		h = h->next;
	}
	return (arr);
}

/**
 * @brief Removes node from environment list
 * 
 * @param head Pointer to list head pointer
 * @param name Name of variable to remove
 * 
 * @note If variable not found, does nothing
 * Properly handles removal of head node
 * Frees all memory associated with removed node
 */
void	remove_env_node(t_env **head, char *name)
{
	t_env	*current;
	t_env	*prev;

	if (!head || !*head)
		return ;
	current = *head;
	prev = NULL;
	while (current)
	{
		if (ft_strcmp(current->name, name) == 0)
		{
			if (prev)
				prev->next = current->next;
			else
				*head = current->next;
			free(current->name);
			free(current->value);
			free(current);
			return ;
		}
		prev = current;
		current = current->next;
	}
}
