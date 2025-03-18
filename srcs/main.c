/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elehtone <elehtone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 18:26:32 by elehtone          #+#    #+#             */
/*   Updated: 2025/03/18 18:09:29 by elehtone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/minishell.h"

int main(int argc, char **argv, char **envp)
{
    //char *command;
    t_lexer	lex_data;
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
	
    	/*Test to see if I can get IFS to work, but likely not important
	char *ifs = getenv("IFS");
	printf("env as pulled: %s\n", ifs);
	if (ifs)
	{
		lex_data.IFS = strdup(ifs);
		printf("env: %s\n", lex_data.IFS);
	}
	else
	{
		lex_data.IFS = strdup(" \t\n");
		printf("ifs not found, using defaults: %s\n", lex_data.IFS);
	}*/

        // Parse the command
    	parse_line(&lex_data, " aa >> BBBB | ccc < '$DDD' | eeeee \"TO OL\" | $ff << | GGGG \"$HH II\" ");
	//populate relevant parts or t_command
	//populate_command_struct(&lex_data, );

	//Temp free()s
	unfun_add_entry_fail(&lex_data, 0);

	// Execute the command
        // ...
        //ft_printf("%s \n", command);
        //free(command);
    //}
    // ...
}
