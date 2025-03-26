/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbyrne <mbyrne@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 16:15:12 by mbyrne            #+#    #+#             */
/*   Updated: 2025/03/26 15:14:37 by mbyrne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	handle_file_open_error(t_command *cmd, char *filename)
{
	ft_putstr_fd("minishell: ", 2);
	perror(filename);
	free(filename);
	cmd->error = 1;
	return (ERROR);
}

void	clean_exit(t_mini *mini)
{
	int	exit_status;

	if (!mini)
		exit(EXIT_FAILURE);
	exit_status = mini->exit_status;
	if (mini->env)
		free_env_list(mini->env);
	if (mini->commands)
		free_commands(mini);
	exit(exit_status);
}

/* Complete lexer cleanup */
void	cleanup_lexer(t_lexer *lexer)
{
	if (!lexer)
		return ;
	if (lexer->dictionary)
		ft_lstclear(&lexer->dictionary, free_token);
}
