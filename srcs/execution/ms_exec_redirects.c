/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirect.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbyrne <mbyrne@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 11:16:39 by mbyrne            #+#    #+#             */
/*   Updated: 2025/03/18 11:01:55 by mbyrne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// Function prototypes for this file only
static int setup_input_redir(t_command *cmd);
static int setup_output_redir(t_command *cmd, int append);
static int setup_heredoc(t_command *cmd);
static char *remove_quotes(const char *str);

// Handle all redirections for a command
int handle_redirection(t_mini *mini, t_command *cmd)
{
    (void)mini;
    
    // We need to handle all redirections, not just one
    // This is a placeholder - we need to modify the parser to store all redirections
    // For now, let's just handle the one we have
    if (cmd->redir_file)
    {
        int redir_error = 0;
        switch (cmd->redir_type)
        {
            case TOKEN_REDIR_IN:
                if (setup_input_redir(cmd) == ERROR)
                    redir_error = 1;
                break;
            case TOKEN_REDIR_OUT:
                if (setup_output_redir(cmd, 0) == ERROR)
                    redir_error = 1;
                break;
            case TOKEN_REDIR_APPEND:
                if (setup_output_redir(cmd, 1) == ERROR)
                    redir_error = 1;
                break;
            case TOKEN_HEREDOC:
                if (setup_heredoc(cmd) == ERROR)
                    redir_error = 1;
                break;
            default:
                break;
        }
        if (redir_error)
        {
            mini->exit_status = 1;
            return (ERROR);
        }
    }

    // Apply the redirections
    if (cmd->has_input_redir && cmd->fd_in != STDIN_FILENO)
    {
        if (dup2(cmd->fd_in, STDIN_FILENO) == -1)
            return (ERROR);
        if (!cmd->is_heredoc)
            close(cmd->fd_in);
    }
    
    if (cmd->has_output_redir && cmd->fd_out != STDOUT_FILENO)
    {
        if (dup2(cmd->fd_out, STDOUT_FILENO) == -1)
            return (ERROR);
        close(cmd->fd_out);
    }
    
    return (SUCCESS);
}

// Handle regular input redirection (<)
static int setup_input_redir(t_command *cmd)
{
    int fd;

    if (!cmd->redir_file)
        return (ERROR);
    
    // Remove quotes from filename if present
    char *unquoted_file = remove_quotes(cmd->redir_file);
    if (!unquoted_file)
        return (ERROR);
    
    fd = open(unquoted_file, O_RDONLY);
    if (fd == -1)
    {
        ft_putstr_fd("minishell: ", 2);
        perror(unquoted_file);
        free(unquoted_file);
        return (ERROR);
    }
    
    free(unquoted_file);
    cmd->fd_in = fd;
    cmd->has_input_redir = 1;
    return (SUCCESS);
}

// Handle heredoc (<<)
static int setup_heredoc(t_command *cmd)
{
    int     pipe_fd[2];
    char    *line;
    char    *expanded_line;
    char    *unquoted_delimiter;

    if (pipe(pipe_fd) == -1)
        return (ERROR);

    // Remove quotes from delimiter if present
    unquoted_delimiter = remove_quotes(cmd->redir_file);
    if (!unquoted_delimiter)
    {
        close(pipe_fd[0]);
        close(pipe_fd[1]);
        return (ERROR);
    }

    while (1)
    {
        line = readline("> ");
        if (!line)
        {
            close(pipe_fd[0]);
            close(pipe_fd[1]);
            free(unquoted_delimiter);
            return (ERROR);
        }
        
        if (ft_strcmp(line, unquoted_delimiter) == 0)
        {
            free(line);
            free(unquoted_delimiter);
            break;
        }
        
        // Expand variables in the heredoc content
        expanded_line = expand_variables(cmd->mini, line, 0);
        free(line);
        if (!expanded_line)
        {
            close(pipe_fd[0]);
            close(pipe_fd[1]);
            free(unquoted_delimiter);
            return (ERROR);
        }
        
        write(pipe_fd[1], expanded_line, ft_strlen(expanded_line));
        write(pipe_fd[1], "\n", 1);
        free(expanded_line);
    }

    close(pipe_fd[1]);
    cmd->fd_in = pipe_fd[0];
    cmd->has_input_redir = 1;
    cmd->is_heredoc = 1;
    return (SUCCESS);
}

// Handle output redirection (> and >>)
static int setup_output_redir(t_command *cmd, int append)
{
    int fd;
    int flags;

    flags = O_WRONLY | O_CREAT;
    if (append)
        flags |= O_APPEND;
    else
        flags |= O_TRUNC;
    
    if (!cmd->redir_file)
        return (ERROR);
    
    // Remove quotes from filename if present
    char *unquoted_file = remove_quotes(cmd->redir_file);
    if (!unquoted_file)
        return (ERROR);
    
    fd = open(unquoted_file, flags, 0644);
    if (fd == -1)
    {
        ft_putstr_fd("minishell: ", 2);
        perror(unquoted_file);
        free(unquoted_file);
        return (ERROR);
    }
    
    free(unquoted_file);
    cmd->fd_out = fd;
    cmd->has_output_redir = 1;
    cmd->append = append;
    return (SUCCESS);
}

// Restore original file descriptors
void restore_io(int saved_stdin, int saved_stdout) 
{
    if (saved_stdin != STDIN_FILENO) {
        dup2(saved_stdin, STDIN_FILENO);
        close(saved_stdin);
    }
    
    if (saved_stdout != STDOUT_FILENO) {
        dup2(saved_stdout, STDOUT_FILENO);
        close(saved_stdout);
    }
}

// Add helper function at the top of the file
static char *remove_quotes(const char *str)
{
    // More robust approach - handle quotes properly
    int i, j;
    int len = ft_strlen(str);
    char *result = malloc(len + 1);
    int in_single_quote = 0;
    int in_double_quote = 0;
    
    if (!result)
        return (NULL);
    
    i = 0;
    j = 0;
    while (str[i])
    {
        // Handle quotes based on context
        if (str[i] == '\'' && !in_double_quote)
        {
            in_single_quote = !in_single_quote;
        }
        else if (str[i] == '"' && !in_single_quote)
        {
            in_double_quote = !in_double_quote;
        }
        else
        {
            result[j++] = str[i];
        }
        i++;
    }
    result[j] = '\0';
    return (result);
}