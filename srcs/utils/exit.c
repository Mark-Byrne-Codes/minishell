
#include "../../includes/minishell.h"

void clean_exit(t_mini *mini)
{
	if (mini)
	{
		if (mini->commands)
			free_command(mini->commands);
		if (mini->tokens)
			free_tokens(mini->tokens);
		if (mini->env)
			free_env_list(mini->env);
    }
	exit(mini->exit_status);
}
