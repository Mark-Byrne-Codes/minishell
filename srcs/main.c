/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elehtone <elehtone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 18:26:32 by elehtone          #+#    #+#             */
/*   Updated: 2025/03/11 14:26:22 by elehtone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/minishell.h"

int main(int argc, char **argv, char **envp)
{
    //char *command;
    t_lexer	*lex_data;
    (void)*envp;
    (void)argv;
    
    if (argc != 1)
	    return (1);
    // ...
    //while (1)
    //{
        // ...
        // Read the command from the user
        //command = readline("minishell> ");
        // ...
        // Parse the command
	lex_data = malloc(sizeof(t_lexer));
    	parse_line(lex_data, "cat << infile | grep < 'fool' | grep > \"tool\" | wc >> ");
        // Execute the command
        // ...
        //ft_printf("%s \n", command);
        //free(command);
    //}
    // ...
}
