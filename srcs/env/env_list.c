/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_list.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbyrne <mbyrne@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 11:33:17 by mbyrne            #+#    #+#             */
/*   Updated: 2025/03/23 12:02:15 by mbyrne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
