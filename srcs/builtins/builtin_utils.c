/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbyrne <mbyrne@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 09:17:23 by mbyrne            #+#    #+#             */
/*   Updated: 2025/03/06 12:39:58 by mbyrne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../includes/minishell.h"

bool is_builtin(char *command)
{
    if (command == NULL)
        return (false);
    if (ft_strcmp(command, "echo") == 0 ||
        ft_strcmp(command, "cd") == 0 ||
        ft_strcmp(command, "pwd") == 0 ||
        ft_strcmp(command, "export") == 0 ||
        ft_strcmp(command, "unset") == 0 ||
        ft_strcmp(command, "env") == 0 ||
        ft_strcmp(command, "exit") == 0)
    {
        return (true);
    }
    return(false);
}