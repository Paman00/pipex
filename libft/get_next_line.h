/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migugar2 <migugar2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 13:02:57 by migugar2          #+#    #+#             */
/*   Updated: 2024/10/27 13:18:58 by migugar2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1
# endif

# ifndef MAX_FD
#  define MAX_FD 1024
# endif

# include <stddef.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include "libft.h"

char	*get_new_saved(char **saved, char *new_line, size_t line_len);
char	*extract_line(char **saved);
char	*read_until_eol_eof(int fd, char **saved, char **buffer);

char	*get_next_line(int fd);
char	*get_next_line_max(int fd);

#endif
