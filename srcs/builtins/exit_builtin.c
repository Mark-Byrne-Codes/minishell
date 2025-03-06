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