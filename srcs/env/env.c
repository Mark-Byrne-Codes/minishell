/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbyrne <mbyrne@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 11:33:17 by mbyrne            #+#    #+#             */
/*   Updated: 2025/03/26 15:58:24 by mbyrne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * @brief Creates a new environment variable node
 * 
 * @param name Name of the environment variable
 * @param value Value of the environment variable
 * @return t_env* Newly allocated node, or NULL on failure
 * 
 * @note Allocates memory for both name and value strings
 * Caller is responsible for freeing the node using free_env_list()
 */
t_env	*create_env_node(char *name, char *value)
{
	t_env	*new_node;

	new_node = (t_env *)malloc(sizeof(t_env));
	if (!new_node)
		return (NULL);
	new_node->name = ft_strdup(name);
	if (!new_node->name)
	{
		free(new_node);
		return (NULL);
	}
	new_node->value = ft_strdup(value);
	if (!new_node->value)
	{
		free(new_node->name);
		free(new_node);
		return (NULL);
	}
	new_node->next = NULL;
	return (new_node);
}

/**
 * @brief Adds a node to the end of the environment list
 * 
 * @param head Pointer to the head of the list
 * @param new_node Node to add to the list
 * 
 * @note If head is NULL, becomes the new head
 * Maintains the list in order of addition
 */
void	add_env_node(t_env **head, t_env *new_node)
{
	t_env	*current;

	if (!head || !new_node)
		return ;
	if (!*head)
	{
		*head = new_node;
		return ;
	}
	current = *head;
	while (current->next)
		current = current->next;
	current->next = new_node;
}

/**
 * @brief Gets the value of an environment variable
 * 
 * @param env Head of environment list
 * @param key Name of variable to find
 * @return char* Value of variable, or NULL if not found
 * 
 * @note Returns NULL if env list is empty or key is invalid
 * Does not allocate new memory - returns pointer to existing value
 */
char	*get_env_value(t_env *env, char *key)
{
	if (!env || !key || !*key)
		return (NULL);
	while (env)
	{
		if (env->name && ft_strcmp(env->name, key) == 0)
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

/**
 * @brief Finds an environment variable node by name
 * 
 * @param head Head of environment list
 * @param name Name of variable to find
 * @return t_env* Pointer to found node, or NULL if not found
 * 
 * @note Performs exact match comparison
 * Returns first match if multiple variables with same name exist
 */
t_env	*find_env_node(t_env *head, const char *name)
{
	while (head)
	{
		if (ft_strcmp(head->name, name) == 0)
			return (head);
		head = head->next;
	}
	return (NULL);
}

/**
 * @brief Frees the entire environment list
 * 
 * @param head Head of the list to free
 * 
 * @note Safely handles NULL head
 * Frees all associated name and value strings
 */
void	free_env_list(t_env *head)
{
	t_env	*temp;

	while (head != NULL)
	{
		temp = head;
		head = head->next;
		free(temp->name);
		free(temp->value);
		free(temp);
	}
}
