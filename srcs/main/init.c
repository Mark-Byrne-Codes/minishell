/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbyrne <mbyrne@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 09:13:29 by mbyrne            #+#    #+#             */
/*   Updated: 2025/03/26 11:17:18 by mbyrne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * @brief Initializes the command array with default values
 * 
 * @param mini Pointer to the main minishell structure
 * @param num_commands Number of commands to initialize
 * @return int SUCCESS on success, ERROR on allocation failure
 * 
 * @note Sets up default file descriptors (stdin/stdout) and initializes
 * pipe file descriptors to -1 (invalid). Each command gets a reference
 * back to the main minishell structure.
 */
int	init_commands(t_mini *mini, int num_commands)
{
	int	i;

	mini->num_commands = num_commands;
	mini->commands = ft_calloc(num_commands, sizeof(t_command));
	if (!mini->commands)
		return (ERROR);
	i = 0;
	while (i < num_commands)
	{
		ft_memset(&mini->commands[i], 0, sizeof(t_command));
		mini->commands[i].fd_in = STDIN_FILENO;
		mini->commands[i].fd_out = STDOUT_FILENO;
		mini->commands[i].pipe_read = -1;
		mini->commands[i].pipe_write = -1;
		mini->commands[i].mini = mini;
		i++;
	}
	return (SUCCESS);
}

/**
 * @brief Initializes the main minishell structure
 * 
 * @param mini Pointer to the structure to initialize
 * @param envp Environment variables array
 * @return int 0 on success, 1 on environment copy failure
 * 
 * @note Performs critical initialization including:
 * - Environment variable copying
 * - Signal handler setup
 * - Default file descriptors
 * - Initial state flags
 */
int	init_mini(t_mini *mini, char **envp)
{
	mini->env = copy_env(envp);
	if (!mini->env)
		return (1);
	mini->envp = envp;
	mini->exit_status = 0;
	mini->should_exit = 0;
	setup_signal_handlers();
	return (0);
}

/**
 * @brief Initializes the lexer structure with default values
 * 
 * @param lexer Pointer to the lexer structure to initialize
 * 
 * @note Resets all state flags to 0 and initializes the dictionary to NULL.
 * This prepares the lexer for a new parsing operation.
 */
void	init_lexer(t_lexer *lexer)
{
	lexer->squote = 0;
	lexer->dquote = 0;
	lexer->pipes = 0;
	lexer->vars = 0;
	lexer->red_in = 0;
	lexer->red_delim = 0;
	lexer->red_out = 0;
	lexer->red_append = 0;
	lexer->dictionary = NULL;
}
