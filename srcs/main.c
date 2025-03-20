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
    setup_signal_handlers(mini);
	return (0);
}

void print_commands(t_mini *mini)
{
    int i = 0;
    int j;
    
    printf("\n==== COMMAND DETAILS ====\n");
    while (i < mini->num_commands)
    {
        printf("Command %d:\n", i + 1);
        
        // Print arguments
        j = 0;
        printf("  Arguments:\n");
        if (mini->commands[i].args != NULL)
        {
            while (mini->commands[i].args[j] != NULL)
            {
                printf("    Arg[%d]: %s\n", j, mini->commands[i].args[j]);
                j++;
            }
        }
        else
        {
            printf("    No arguments\n");
        }
        
        // Print file descriptors
        printf("  File descriptors:\n");
        printf("    fd_in: %d\n", mini->commands[i].fd_in);
        printf("    fd_out: %d\n", mini->commands[i].fd_out);
        printf("    pipe_read: %d\n", mini->commands[i].pipe_read);
        printf("    pipe_write: %d\n", mini->commands[i].pipe_write);
        
        // Print flags
        printf("  Flags:\n");
        printf("    is_builtin: %d\n", mini->commands[i].is_builtin);
        printf("    has_input_redir: %d\n", mini->commands[i].has_input_redir);
        printf("    has_output_redir: %d\n", mini->commands[i].has_output_redir);
        printf("    append: %d\n", mini->commands[i].append);
        printf("    is_heredoc: %d\n", mini->commands[i].is_heredoc);
        printf("    exit_status: %d\n", mini->commands[i].exit_status);
        
        printf("\n");
        i++;
    }
    printf("========================\n\n");
}

int parse_and_execute(char *line, t_mini *mini)
{
    t_lexer *lex_data;

    lex_data = ft_calloc(1, sizeof(t_lexer));
    if (!lex_data)
    {
        mini->exit_status = ERROR;
        return (ERROR);
    }
    if (parse_line(lex_data, line, mini) == ERROR)
    {
        free(lex_data);
        mini->exit_status = ERROR;
        return (ERROR);
    }
   //print_commands(mini);
    if (mini->num_commands > 0)
    {
        if (execute_commands(mini) == ERROR)
        {
            free_commands(mini);
            free_tokens(mini);
            free(lex_data);
            mini->exit_status = ERROR;
            return (ERROR);
        }
    }
    free_commands(mini);
    free_tokens(mini);
    free(lex_data);
    return (SUCCESS);
}

void main_loop(t_mini *mini)
{
    char    *line;
    char    *prompt;

    while (!mini->should_exit)
    {
        prompt = get_prompt();
        line = readline(prompt);
        free(prompt);
        if (!line)
        {
            ft_putendl_fd("exit", STDOUT_FILENO);
            break;
        }
        if (*line == '\0')
        {
            free(line);
            continue;
        }
        add_history(line);
        if (parse_and_execute(line, mini) == ERROR)
        {
            free(line);
            continue;
        }
        free(line);
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