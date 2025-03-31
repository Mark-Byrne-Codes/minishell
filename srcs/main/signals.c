/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbyrne <mbyrne@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 13:17:33 by mbyrne            #+#    #+#             */
/*   Updated: 2025/03/31 09:52:17 by mbyrne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * @brief Signal handler for interactive shell mode (Ctrl+C)
 * 
 * Handles SIGINT (Ctrl+C) in the main interactive shell:
 * - Prints a newline
 * - Resets readline's line state
 * - Sets global signal status to 130 (standard shell exit code for SIGINT)
 * 
 * @param signum The signal number (should be SIGINT)
 * 
 * @note Only handles SIGINT - SIGQUIT is ignored in interactive mode
 * @note Maintains proper line editing state with readline functions
 */
void	signal_handler_interactive(int signum)
{
	if (signum == SIGINT)
	{
		write(STDOUT_FILENO, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		g_signal = 130;
	}
}

/**
 * @brief Signal handler for heredoc mode (Ctrl+C)
 * 
 * Handles SIGINT (Ctrl+C) during heredoc input:
 * - Prints a newline
 * - Closes STDIN to break out of readline
 * - Exits with status 130 (standard shell exit code)
 * 
 * @param signum The signal number (should be SIGINT)
 * 
 * @note This handler is specifically for heredoc context
 * @warning The exit() call terminates only the heredoc child process
 */
void	signal_handler_heredoc(int signum)
{
	if (signum == SIGINT)
	{
		close(STDIN_FILENO);
		exit(130);
	}
}

/**
 * @brief Sets up default signal handlers for interactive shell
 * 
 * Configures:
 * - SIGINT (Ctrl+C) to use signal_handler_interactive
 * - SIGQUIT (Ctrl+\\) to be ignored
 * 
 * @note Should be called during shell initialization
 * @note Used for main shell prompt handling
 */
void	setup_signal_handlers(void)
{
	signal(SIGINT, signal_handler_interactive);
	signal(SIGQUIT, SIG_IGN);
}

/**
 * @brief Sets up signal handlers for heredoc processing
 * 
 * Configures:
 * - SIGINT (Ctrl+C) to use signal_handler_heredoc
 * - SIGQUIT (Ctrl+\\) to be ignored
 * 
 * @note Used specifically for heredoc child processes
 * @note Differs from interactive handlers by exiting on SIGINT
 */
void	setup_heredoc_signals(void)
{
	signal(SIGINT, signal_handler_heredoc);
	signal(SIGQUIT, SIG_IGN);
}

/**
 * @brief Restores default signal behavior
 * 
 * Resets both SIGINT and SIGQUIT to default behavior
 * and cleans up readline's internal state.
 * 
 * @note Typically used before executing child processes
 * @note Ensures proper signal propagation to child processes
 */
void	restore_default_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}
