/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbyrne <mbyrne@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 15:39:26 by mbyrne            #+#    #+#             */
/*   Updated: 2025/03/31 15:17:00 by mbyrne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	g_signal;

int	parse_and_execute(char *line, t_mini *mini)
{
	t_lexer	*lex_data;

	lex_data = mini->lexer_data;
	if (!lex_data)
	{
		mini->exit_status = ERROR;
		return (ERROR);
	}
	if (parse_line(lex_data, line, mini) == ERROR)
	{
		mini->exit_status = ERROR;
		return (ERROR);
	}
	if (mini->num_commands > 0 && execute_commands(mini) == ERROR)
	{
		free_commands(mini);
		cleanup_on_token_error(mini->lexer_data, 0);
		free(mini->lexer_data);
		mini->exit_status = ERROR;
		mini->lexer_data = NULL;
		return (ERROR);
	}
	free_commands(mini);
	return (SUCCESS);
}

void	process_input_line(char *line, t_mini *mini)
{
	if (*line == '\0')
	{
		free(line);
		return ;
	}
	add_history(line);
	mini->lexer_data = ft_calloc(1, sizeof(t_lexer));
	if (!mini->lexer_data)
	{
		mini->should_exit = 1;
		free(line);
		return ;
	}
	if (parse_and_execute(line, mini) == ERROR)
	{
		free(line);
		return ;
	}
	free(line);
	if (mini->lexer_data)
	{
		cleanup_lexer(mini->lexer_data);
		free(mini->lexer_data);
		mini->lexer_data = NULL;
	}
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
