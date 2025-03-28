/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbyrne <mbyrne@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 15:39:26 by mbyrne            #+#    #+#             */
/*   Updated: 2025/03/27 10:43:39 by mbyrne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	g_signal;

int	parse_and_execute(char *line, t_mini *mini, t_lexer *lex_data)
{
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
	if (mini->num_commands > 0 && execute_commands(mini) == ERROR)
	{
		free_commands(mini);
		free(lex_data);
		mini->exit_status = ERROR;
		return (ERROR);
	}
	free_commands(mini);
	free(lex_data);
	return (SUCCESS);
}

void	process_input_line(char *line, t_mini *mini)
{
	t_lexer	*lex_data;

	lex_data = ft_calloc(1, sizeof(t_lexer));
	if (*line == '\0')
	{
		free(line);
		free(lex_data);
		return ;
	}
	add_history(line);
	if (parse_and_execute(line, mini, lex_data) == ERROR)
	{
		free(line);
		if (!lex_data)
			free(lex_data);
		return ;
	}
	free(line);
	if (!lex_data)
		free(lex_data);
}

void	main_loop(t_mini *mini)
{
	char	*line;
	char	*prompt;

	while (!mini->should_exit)
	{
		prompt = get_prompt();
		line = readline(prompt);
		free(prompt);
		if (!line)
		{
			ft_putendl_fd("exit", STDOUT_FILENO);
			break ;
		}
		process_input_line(line, mini);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_mini	mini;

	ft_memset(&mini, 0, sizeof(t_mini));
	if (argc != 1)
	{
		ft_putstr_fd("Usage: ./minishell\n", 2);
		return (1);
	}
	(void)argv;
	g_signal = 0;
	if (init_mini(&mini, envp) != 0)
		return (1);
	main_loop(&mini);
	rl_clear_history();
	clean_exit(&mini);
	return (mini.exit_status);
}
