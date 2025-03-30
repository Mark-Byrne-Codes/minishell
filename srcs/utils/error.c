/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbyrne <mbyrne@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 15:14:33 by mbyrne            #+#    #+#             */
/*   Updated: 2025/03/27 14:10:26 by mbyrne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * @brief Handles file opening errors with proper error messaging
 * 
 * @param cmd Command structure where error will be recorded
 * @param filename Name of the file that failed to open
 * @return int Always returns ERROR (1)
 * 
 * @note Prints error message in format: "minishell: [filename]: [system error]"
 * @note Sets cmd->error flag to indicate failure
 * @note Frees the filename memory
 */
int	handle_file_open_error(t_command *cmd, char *filename)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	perror(filename);
	free(filename);
	cmd->error = 1;
	return (ERROR);
}

/**
 * @brief Cleanly exits the shell with proper resource cleanup
 * 
 * @param mini Main shell structure containing all state
 * 
 * @note Performs the following cleanup:
 * - Frees environment variable list
 * - Frees command structures
 * - Uses stored exit_status for final exit code
 * @warning This function does not return - it terminates the process
 */
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
	if (mini->lexer_data)
	{
		cleanup_on_token_error(mini->lexer_data, 0);
		free(mini->lexer_data);
		mini->lexer_data = NULL;
	}
	exit(exit_status);
}

/**
 * @brief Completely cleans up lexer resources
 * 
 * @param lexer Lexer structure to clean up
 * 
 * @note Safely handles NULL lexer pointer
 * @note Clears the token dictionary using free_token as destructor
 */
void	cleanup_lexer(t_lexer *lexer)
{
	if (!lexer)
		return ;
	if (lexer->dictionary)
		ft_lstclear(&lexer->dictionary, free_token);
}
