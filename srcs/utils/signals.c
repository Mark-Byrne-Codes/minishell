#include "../../includes/minishell.h"
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

//need to handle exit code for ctrl -c command

void	signal_handler_interactive(int signum)
{
	if (signum == SIGINT)
	{
		write(STDOUT_FILENO, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	} 
}

void	signal_handler_heredoc(int signum)
{
	if (signum == SIGINT)
	{
		write(STDOUT_FILENO, "\n", 1);
		exit(130);
	}
}

void	setup_signal_handlers(t_mini *mini)
{
	(void)mini;
	signal(SIGINT, signal_handler_interactive);
	signal(SIGQUIT, SIG_IGN);
}

void	restore_default_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}