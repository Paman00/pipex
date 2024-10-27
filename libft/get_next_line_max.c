/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_max.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migugar2 <migugar2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 16:41:29 by migugar2          #+#    #+#             */
/*   Updated: 2024/10/27 13:20:52 by migugar2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*get_next_line_max(int fd)
{
	static char	*saved[MAX_FD];
	char		*buffer;
	char		*line;

	if (fd < 0 || fd > MAX_FD || BUFFER_SIZE <= 0)
		return (NULL);
	if (saved[fd] == NULL)
		saved[fd] = ft_strdup("");
	if (saved[fd] == NULL)
		return (NULL);
	buffer = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (buffer == NULL)
		return (ft_free_str(&saved[fd]));
	saved[fd] = read_until_eol_eof(fd, &saved[fd], &buffer);
	if (saved[fd] == NULL)
		return (ft_free_str(&buffer), NULL);
	line = extract_line(&saved[fd]);
	ft_free_str(&buffer);
	return (line);
}
