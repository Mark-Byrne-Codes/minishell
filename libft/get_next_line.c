/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 15:58:51 by mbyrne            #+#    #+#             */
/*   Updated: 2024/12/12 07:21:50 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"	

#ifndef BUFFER_SIZE
# define BUFFER_SIZE 42
#endif
/*Reads from the file descriptor and 
appends new data to the buffer until a newline or EOF is found. */
static char	*read_and_update_buf(int fd, char *read_buf, char *full_buf)
{
	int		bytes_read;
	char	*updated_buffer;

	while (!ft_strchr(full_buf, '\n'))
	{
		bytes_read = read(fd, read_buf, BUFFER_SIZE);
		if (bytes_read < 0)
		{
			free(full_buf);
			return (NULL);
		}
		if (bytes_read == 0 && full_buf[0] == '\0')
		{
			free(full_buf);
			return (NULL);
		}
		if (bytes_read == 0)
			break ;
		read_buf[bytes_read] = '\0';
		updated_buffer = ft_strjoin(full_buf, read_buf);
		free(full_buf);
		full_buf = updated_buffer;
	}
	return (full_buf);
}

/*Extracts the next line from the buffer and 
updates the starting index for the leftover buffer.*/
static char	*extract_next_line(char *full_buffer, int *start_next_line)
{
	char	*current_line;
	int		index;

	index = 0;
	if (full_buffer[0] == '\0')
		return (NULL);
	while (full_buffer[index] && full_buffer[index] != '\n')
		index++;
	if (full_buffer[index] == '\n')
		*start_next_line = index + 1;
	else
		*start_next_line = index;
	current_line = ft_substr(full_buffer, 0, *start_next_line);
	return (current_line);
}

/* Saves the remaining buffer after extracting a line and 
frees the old buffer. */
static char	*save_remaining_buffer(char *full_buffer, int start)
{
	char	*remaining_buffer;
	int		remaining_length;

	remaining_length = ft_strlen(full_buffer) - start;
	if (remaining_length <= 0)
	{
		free(full_buffer);
		return (NULL);
	}
	remaining_buffer = ft_substr(full_buffer, start, remaining_length);
	free(full_buffer);
	return (remaining_buffer);
}

/* Mangages the process of reading a file line by line, 
managing memory, and returning lines. */
char	*get_next_line(int fd)
{
	char		*read_buffer;
	char		*line_to_return;
	static char	*stored_buffer;
	int			start_next_line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	if (!stored_buffer)
		stored_buffer = ft_strdup("");
	read_buffer = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!read_buffer)
		return (NULL);
	stored_buffer = read_and_update_buf(fd, read_buffer, stored_buffer);
	free(read_buffer);
	if (!stored_buffer)
		return (NULL);
	line_to_return = extract_next_line(stored_buffer, &start_next_line);
	stored_buffer = save_remaining_buffer(stored_buffer, start_next_line);
	return (line_to_return);
}
