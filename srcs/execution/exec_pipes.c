/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipes.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbyrne <mbyrne@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 11:16:37 by mbyrne            #+#    #+#             */
/*   Updated: 2025/03/26 12:26:46 by mbyrne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * @brief Sets up pipes between commands in a pipeline
 * 
 * @param mini Pointer to minishell structure
 * @param cmd_idx Index of current command
 * @return int SUCCESS on success, ERROR on pipe creation failure
 * 
 * @note Creates pipes between commands except for the last one.
 * Sets pipe_read and pipe_write file descriptors for adjacent commands.
 */
int	setup_pipes(t_mini *mini, int cmd_idx)
{
	int	pipe_fd[2];

	if (cmd_idx < mini->num_commands - 1)
	{
		if (pipe(pipe_fd) == -1)
			return (ERROR);
		mini->commands[cmd_idx].pipe_write = pipe_fd[1];
		mini->commands[cmd_idx + 1].pipe_read = pipe_fd[0];
	}
	else
		mini->commands[cmd_idx].pipe_write = -1;
	return (SUCCESS);
}

/**
 * @brief Connects command's input/output to appropriate pipes
 * 
 * @param mini Pointer to minishell structure
 * @param cmd_idx Index of current command
 * 
 * @note Redirects stdin/stdout to pipe file descriptors
 * and closes the original pipe ends in the child process.
 */
static void	connect_pipes(t_mini *mini, int cmd_idx)
{
	t_command	*cmd;

	cmd = &mini->commands[cmd_idx];
	if (cmd->pipe_read != -1)
	{
		dup2(cmd->pipe_read, STDIN_FILENO);
		close(cmd->pipe_read);
	}
	if (cmd->pipe_write != -1)
	{
		dup2(cmd->pipe_write, STDOUT_FILENO);
		close(cmd->pipe_write);
	}
}

/**
 * @brief Closes pipe file descriptors for current command
 * 
 * @param mini Pointer to minishell structure
 * @param cmd_idx Index of current command
 * 
 * @note Ensures no file descriptor leaks in parent process.
 * Skips closing stdin/stdout if they weren't modified.
 */
static void	close_pipes(t_mini *mini, int cmd_idx)
{
	t_command	*cmd;

	cmd = &mini->commands[cmd_idx];
	if (cmd->pipe_read != STDIN_FILENO)
		close(cmd->pipe_read);
	if (cmd->pipe_write != STDOUT_FILENO)
		close(cmd->pipe_write);
}

/**
 * @brief Executes a single command in the pipeline
 * 
 * @param mini Pointer to minishell structure
 * @param cmd_idx Index of command to execute
 * 
 * @note Handles both builtin and external commands.
 * For external commands, finds full path and executes.
 * Exits child process with appropriate status on errors.
 */
static void	exec_pipe_command(t_mini *mini, int cmd_idx)
{
	char	*path;

	if (mini->commands[cmd_idx].is_builtin)
		exit(execute_builtin(mini, cmd_idx));
	path = get_command_path(mini->commands[cmd_idx].args[0], mini->env);
	if (!path)
	{
		perror("command not found");
		mini->exit_status = 127;
		clean_exit(mini);
	}
	execve(path, mini->commands[cmd_idx].args, env_list_to_array(mini->env));
	perror("execve failed");
	mini->exit_status = 126;
	clean_exit(mini);
}

/**
 * @brief Executes all commands in a pipeline
 * 
 * @param mini Pointer to minishell structure
 * @return int Exit status of pipeline or ERROR on failure
 * 
 * @note For each command:
 * 1. Sets up pipes
 * 2. Forks child process
 * 3. Connects pipes and executes command
 * 4. Closes pipes in parent
 * 5. Waits for all children to complete
 */
int	execute_pipeline(t_mini *mini)
{
	pid_t	pid;
	int		i;

	i = 0;
	while (i < mini->num_commands)
	{
		if (setup_pipes(mini, i) == ERROR)
			return (ERROR);
		pid = fork();
		if (pid == 0)
		{
			connect_pipes(mini, i);
			handle_redirection(mini, &mini->commands[i]);
			exec_pipe_command(mini, i);
		}
		else if (pid < 0)
			return (ERROR);
		close_pipes(mini, i);
		i++;
	}
	return (wait_for_children(mini, SUCCESS));
}
