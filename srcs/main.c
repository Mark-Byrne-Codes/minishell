# include "../includes/minishell.h"


static int	init_mini(t_mini *mini, int argc, char **argv, char **envp)
{
	if (argc != 1)
	{
		ft_putstr_fd("Usage: ./minishell\n", 2);
		return (1);
	}
    (void)argv;
    mini->env = copy_env(envp);
    if (!mini->env)
	    return (1);
    mini->envp = envp;
	mini->exit_status = 0;
	mini->fd_in = 0;
	mini->fd_out = 1;
	mini->should_exit = 0;
    mini->is_builtin = 0;
    // setup_signal_handlers();
	return (0);
}

void main_loop(t_mini *mini)
{
    char *line;
    t_lexer	*lex_data;

    while (!mini->should_exit)
    {
        line = readline("minishell> ");
        if (!line)
            break;
        if (*line)
            add_history(line);

        lex_data = malloc(sizeof(t_lexer));
        parse_line(lex_data, line);
        if (ft_strncmp(line, "exit", 4) == 0)
            mini->should_exit = 1;
        ft_printf("line: %s\n", line);
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
    clean_exit(&mini);
	return (mini.exit_status);
}
