# include "../includes/minishell.h"


static int	init_mini(t_mini *mini, int argc, char **argv, char **envp)
{
	if (argc != 1)
	{
		ft_putstr_fd("Usage: ./minishell\n", 2);
		return (1);
	}
    (void)argv;
    // copy the environment in env list
    mini->envp = envp;
	// if (!mini->env)
	// 	return (1);
	mini->exit_status = 0;
	mini->fd_in = 0;
	mini->fd_out = 1;
	mini->should_exit = 0;
    mini->is_builtin = 0;
    // setup signal handlers for ctrl-c and such
	return (0);
}

void parse_line(t_mini mini, char *line)
{
    t_command *commands;
    // parse the command

    //execute_command(mini, commands);
}

void main_loop(t_mini *mini)
{
    char *line;

    while (!mini->should_exit)
    {
        line = readline("minishell> ");
        if (!line)
            break;
        if (*line)
        //     add_history(line);
        //parse_line(mini, line);
        free (line);
    }
}


int	main(int argc, char **argv, char **envp)
{
	t_mini	mini;

	ft_memset(&mini, 0, sizeof(t_mini));
	if (init_mini(&mini, argc, argv, envp) != 0)
		return (1);
	main_loop(&mini);
	rl_clear_history();
	return (mini.exit_status);
}
