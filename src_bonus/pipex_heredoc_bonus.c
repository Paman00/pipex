/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_heredoc_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migugar2 <migugar2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 21:23:41 by migugar2          #+#    #+#             */
/*   Updated: 2024/11/18 00:27:41 by migugar2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

char	*create_tmpfile_name(char *basename)
{
	char	*tmpfile_name;
	char	*tmp;
	size_t	i;

	tmpfile_name = ft_strdup(basename);
	if (tmpfile_name == NULL)
		return (seterrno(ENOMEM), NULL);
	if (access(tmpfile_name, F_OK) == -1)
		return (seterrno(0), tmpfile_name);
	i = 0;
	tmp = NULL;
	while (i <= 9)
	{
		ft_freestr(&tmp);
		ft_freestr(&tmpfile_name);
		tmp = ft_itoa(i++);
		if (tmp == NULL)
			return (seterrno(ENOMEM), NULL);
		tmpfile_name = ft_strjoin(basename, tmp);
		if (tmpfile_name == NULL)
			return (ft_freestr(&tmp), seterrno(ENOMEM), NULL);
		if (access(tmpfile_name, F_OK) == -1)
			return (ft_freestr(&tmp), seterrno(0), tmpfile_name);
	}
	return (ft_freestr(&tmpfile_name), ft_freestr(&tmp), seterrno(17), NULL);
}

char	*read_in_tmpfile(int in_fd, char *limiter, size_t limiter_len)
{
	char	*tmpfile_name;
	int		out_fd;
	char	*line;

	tmpfile_name = create_tmpfile_name("tmpfile_read");
	if (tmpfile_name == NULL)
		exit(error_handler(0, "tmpfile_read"));
	out_fd = open(tmpfile_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (out_fd == -1)
		exit(error_handler_free(0, tmpfile_name));
	while (1)
	{
		ft_printf(PS2);
		line = get_next_line(in_fd);
		if (line == NULL && errno != 0)
			return (ft_close(&out_fd), ft_freestr(&line),
				ft_freestr(&tmpfile_name), exit(error_handler(0, NULL)), NULL);
		if (line == NULL || (ft_strlen(line) - 1 == limiter_len
				&& ft_strncmp(line, limiter, limiter_len) == 0))
			break ;
		ft_printf_fd(out_fd, "%s", line);
		ft_freestr(&line);
	}
	return (ft_close(&out_fd), ft_freestr(&line), tmpfile_name);
}

int	here_doc(int argc, char *argv[], char **envp)
{
	char	*tmpfile;
	int		last_status;

	tmpfile = read_in_tmpfile(0, argv[2], ft_strlen(argv[2]));
	if (tmpfile == NULL)
		exit(error_handler(0, NULL));
	argv[2] = tmpfile;
	last_status = pipex(argc - 1, argv + 1, envp,
			O_CREAT | O_WRONLY | O_APPEND);
	unlink(tmpfile);
	return (ft_freestr(&tmpfile), last_status);
}
