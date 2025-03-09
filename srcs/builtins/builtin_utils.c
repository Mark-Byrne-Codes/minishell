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

int execute_builtin(t_mini *mini, t_command *command)
{
    int ret;
    // ...
    if (ft_strcmp(command, "echo") == 0)
        ret = ft_echo(command->argv);
    if (ft_strcmp(command, "pwd") == 0)
        ret = ft_pwd();
    if (ft_strcmp(command, "env") == 0)
        ret = ft_env(mini);

    // ...
    return (ret);
}