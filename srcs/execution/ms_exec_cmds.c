/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbyrne <mbyrne@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 11:16:34 by mbyrne            #+#    #+#             */
/*   Updated: 2025/03/18 11:00:15 by mbyrne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	execute_commands(t_mini *mini)
{
	int	i;
	int	status;
	int	pipe_failed;
	int	saved_stdin;
	int	saved_stdout;

	i = -1;
	pipe_failed = 0;
	while (++i < mini->num_commands)
	{
		saved_stdin = dup(STDIN_FILENO);
		saved_stdout = dup(STDOUT_FILENO);
		if (saved_stdin == -1 || saved_stdout == -1)
			return (ERROR);

		if (setup_pipes(mini, i) == ERROR)
		{
			restore_io(saved_stdin, saved_stdout);
			return (ERROR);
		}

		if (i < mini->num_commands - 1)
		{
			if (dup2(mini->commands[i].pipe_write, STDOUT_FILENO) == -1)
				return (ERROR);
		}
		if (i > 0)
		{
			if (dup2(mini->commands[i].pipe_read, STDIN_FILENO) == -1)
				return (ERROR);
		}

		if (mini->commands[i].is_builtin && mini->num_commands == 1)
		{
			status = execute_builtin(mini, i);
		}
		else
		{
			status = launch_external(mini, i);
		}

		if (status == ERROR)
			pipe_failed = 1;

		if (mini->commands[i].pipe_write != -1)
			close(mini->commands[i].pipe_write);
		if (mini->commands[i].pipe_read != -1)
			close(mini->commands[i].pipe_read);

		restore_io(saved_stdin, saved_stdout);
	}
	if (pipe_failed)
		status = wait_for_children(mini, ERROR);
	else
		status = wait_for_children(mini, status);
	return (status);
}

int	execute_builtin(t_mini *mini, int cmd_idx)
{
	t_command	*cmd;

	cmd = &mini->commands[cmd_idx];
	if (ft_strcmp(cmd->args[0], "echo") == 0)
		return (echo_builtin(cmd->args));
	else if (ft_strcmp(cmd->args[0], "cd") == 0)
		return (cd_builtin(mini, cmd->args));
	else if (ft_strcmp(cmd->args[0], "pwd") == 0)
		return (pwd_builtin());
	else if (ft_strcmp(cmd->args[0], "export") == 0)
		return (export_builtin(mini, cmd->args));
	else if (ft_strcmp(cmd->args[0], "unset") == 0)
		return (unset_builtin(mini, cmd->args));
	else if (ft_strcmp(cmd->args[0], "env") == 0)
		return (env_builtin(mini));
	else if (ft_strcmp(cmd->args[0], "exit") == 0)
		return (exit_builtin(mini, cmd->args));
	return (SUCCESS);
}

int	launch_external(t_mini *mini, int cmd_idx)
{
	pid_t	pid;
	char	*cmd_path;

	if (mini->commands[cmd_idx].is_builtin)
	{
		pid = fork();
		if (pid == 0)
		{
			if (cmd_idx > 0 && mini->commands[cmd_idx].pipe_read != -1)
				dup2(mini->commands[cmd_idx].pipe_read, STDIN_FILENO);
			if (cmd_idx < mini->num_commands - 1 && mini->commands[cmd_idx].pipe_write != -1)
				dup2(mini->commands[cmd_idx].pipe_write, STDOUT_FILENO);
			exit(execute_builtin(mini, cmd_idx));
		}
		else if (pid < 0)
			return (ERROR);
		mini->last_pid = pid;
		return (SUCCESS);
	}

	if (!mini->commands[cmd_idx].args || !mini->commands[cmd_idx].args[0])
		return (ERROR);

	cmd_path = get_command_path(mini->commands[cmd_idx].args[0], mini->env);
	if (!cmd_path)
	{
		if (cmd_idx == 0)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(mini->commands[cmd_idx].args[0], 2);
			ft_putendl_fd(": command not found", 2);
		}
		mini->exit_status = 127;
		return (ERROR);
	}

	pid = fork();
	if (pid == 0)
	{
		if (cmd_idx > 0 && mini->commands[cmd_idx].pipe_read != -1)
			dup2(mini->commands[cmd_idx].pipe_read, STDIN_FILENO);
		if (cmd_idx < mini->num_commands - 1 && mini->commands[cmd_idx].pipe_write != -1)
			dup2(mini->commands[cmd_idx].pipe_write, STDOUT_FILENO);
		execve(cmd_path,
			mini->commands[cmd_idx].args, env_list_to_array(mini->env));
		if (cmd_idx == 0)
		{
			ft_putstr_fd("minishell: ", 2);
			perror(mini->commands[cmd_idx].args[0]);
		}
		if (errno == ENOENT)
			exit(127);
		exit(126);
	}
	else if (pid < 0)
	{
		free(cmd_path);
		return (ERROR);
	}
	free(cmd_path);
	mini->last_pid = pid;
	return (SUCCESS);
}

int wait_for_children(t_mini *mini, int last_status)
{
	int	status;
	int	last_exit_status = 0;
	pid_t	pid;

	while ((pid = waitpid(-1, &status, WUNTRACED)) > 0)
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
	}
	if (last_status != ERROR)
		return (last_exit_status);
	return (mini->exit_status = last_status);
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