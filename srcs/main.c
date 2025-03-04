# include "../includes/minishell.h"

int main(int argc, char **argv, char **envp)
{
    (void)*envp;
    (void)argc;
    (void)argv;
    char *command;
    // ...
    while (1)
    {
        // ...
        // Read the command from the user
        command = readline("minishell> ");
        // ...
        // Parse the command
        // ...
        // Execute the command
        // ...
        ft_printf("%s \n", command);
        free(command);
    }
    // ...
}