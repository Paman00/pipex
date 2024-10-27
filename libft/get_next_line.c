/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migugar2 <migugar2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 16:18:35 by migugar2          #+#    #+#             */
/*   Updated: 2024/10/27 13:24:22 by migugar2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*get_new_saved(char **saved, char *new_line, size_t line_len)
{
	char	*new_saved;
	char	*line;

	line = ft_substr(*saved, 0, line_len);
	if (line == NULL)
		return (ft_free_str(saved));
	new_line++;
	if (new_line[0] != '\0')
	{
		new_saved = ft_strdup(new_line);
		if (new_saved == NULL)
			return (ft_free_str(&line), ft_free_str(saved));
	}
	else
		new_saved = NULL;
	ft_free_str(saved);
	*saved = new_saved;
	return (line);
}

char	*extract_line(char **saved)
{
	char	*new_line;
	char	*line;

	new_line = ft_strchr(*saved, '\n');
	if (new_line != NULL)
		return (get_new_saved(saved, new_line, new_line - *saved + 1));
	if ((*saved)[0] == '\0')
		return (ft_free_str(saved));
	line = *saved;
	*saved = NULL;
	return (line);
}

char	*read_until_eol_eof(int fd, char **saved, char **buffer)
{
	ssize_t		bytes_read;
	char		*temp;
	char		*compared;

	bytes_read = 1;
	compared = *saved;
	while (bytes_read > 0 && ft_strchr(compared, '\n') == NULL)
	{
		bytes_read = read(fd, *buffer, BUFFER_SIZE);
		if (bytes_read < 0)
			return (ft_free_str(saved), ft_free_str(buffer));
		if (bytes_read > 0)
		{
			(*buffer)[bytes_read] = '\0';
			compared = *buffer;
			temp = ft_strjoin(*saved, *buffer);
			if (temp == NULL)
				return (ft_free_str(saved), ft_free_str(buffer));
			ft_free_str(saved);
			*saved = temp;
		}
	}
	return (*saved);
}

char	*get_next_line(int fd)
{
	static char	*saved;
	char		*buffer;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	if (saved == NULL)
	{
		saved = ft_strdup("");
		if (saved == NULL)
			return (NULL);
	}
	buffer = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (buffer == NULL)
		return (ft_free_str(&saved));
	buffer[BUFFER_SIZE] = '\0';
	if (read_until_eol_eof(fd, &saved, &buffer) == NULL)
		return (ft_free_str(&buffer));
	ft_free_str(&buffer);
	line = extract_line(&saved);
	return (line);
}
