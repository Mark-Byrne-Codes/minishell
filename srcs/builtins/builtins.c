# include "../../includes/minishell.h"

int execute_builtin(t_mini *mini, char *command, char **args)
{
    (void)mini;
    // ...
    if (ft_strcmp(command, "exit") == 0)
    {
        ft_exit(args);
        return (1);
    }
    // ...
    return (0);
}