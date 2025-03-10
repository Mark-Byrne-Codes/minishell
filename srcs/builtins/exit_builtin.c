/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbyrne <mbyrne@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 09:46:36 by mbyrne            #+#    #+#             */
/*   Updated: 2025/03/06 09:46:37 by mbyrne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../includes/minishell.h"

void clean_exit(t_mini *mini);

int ft_exit(t_mini *mini, char **args)
{
    int exit_code;


    exit_code = 69;
    (void)args;
    ft_printf("Exiting minishell\n");
    clean_exit(mini);
    return (0);
}