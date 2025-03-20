#include "../../includes/minishell.h"

int	add_argument(t_command *cmd, char *arg, int arg_idx)
{
	char	**new_args;
	int		i;

	new_args = malloc(sizeof(char *) * (arg_idx + 2));
	if (!new_args)
		return (ERROR);
	i = 0;
	while (i < arg_idx)
	{
		new_args[i] = cmd->args[i];
		i++;
	}
	new_args[arg_idx] = ft_strdup(arg);
	if (!new_args[arg_idx])
	{
		free(new_args);
		return (ERROR);
	}
	new_args[arg_idx + 1] = NULL;
	free(cmd->args);
	cmd->args = new_args;
	return (SUCCESS);
}

static void	init_command(t_command *cmd, t_mini *mini)
{
	cmd->args = NULL;
	cmd->fd_in = STDIN_FILENO;
	cmd->fd_out = STDOUT_FILENO;
	cmd->pipe_read = -1;
	cmd->pipe_write = -1;
	cmd->is_builtin = 0;
	cmd->exit_status = 0;
	cmd->has_input_redir = 0;
	cmd->has_output_redir = 0;
	cmd->append = 0;
	cmd->is_heredoc = 0;
	cmd->mini = mini;
	cmd->redir_type = 0;
	cmd->redir_file = NULL;
	cmd->error = 0;
}

int	init_commands(t_mini *mini, int num_commands)
{
	int	i;

	mini->num_commands = num_commands;
	mini->commands = ft_calloc(num_commands, sizeof(t_command));
	if (!mini->commands)
		return (ERROR);
	i = 0;
	while (i < num_commands)
	{
		init_command(&mini->commands[i], mini);
		i++;
	}
	return (SUCCESS);
}

char *get_command_path(char *cmd, t_env *env)
{
	char	*path;
	char	**dirs;
	int		i;

	if (ft_strchr(cmd, '/'))
		return (cmd);
	path = get_env_value(env, "PATH");
	dirs = ft_split(path, ':');
	i = -1;
	while (dirs[++i])
	{
		path = ft_strjoin3(dirs[i], "/", cmd);
		if (access(path, X_OK) == 0)
		{
			free_string_array(dirs);
			return (path);
		}
		free(path);
	}
	free_string_array(dirs);
	return (NULL);
}

int	wait_for_children(t_mini *mini, int last_status)
{
	int		status;
	int		last_exit_status;
	pid_t	pid;

	last_exit_status = 0;
	pid = waitpid(-1, &status, WUNTRACED);
	while (pid > 0)
	{
		if (pid == mini->last_pid)
		{
			if (WIFEXITED(status))
			{
				mini->exit_status = WEXITSTATUS(status);
				last_exit_status = mini->exit_status;
			}
			else if (WIFSIGNALED(status))
				mini->exit_status = 128 + WTERMSIG(status);
		}
		pid = waitpid(-1, &status, WUNTRACED);
	}
	if (last_status != ERROR)
		return (last_exit_status);
	mini->exit_status = last_status;
	return (mini->exit_status);
}
