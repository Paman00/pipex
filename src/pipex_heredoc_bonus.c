/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_heredoc_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migugar2 <migugar2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 19:24:22 by migugar2          #+#    #+#             */
/*   Updated: 2024/10/27 19:35:18 by migugar2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex_bonus.h"

char	*create_file_here_doc(int argc, char *argv[])
{
	int		in_fd;
	char	*line;
	size_t	limiter_len;

	in_fd = open("temp_file", O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (in_fd == -1)
		exit_pipex(EXIT_FAILURE, errno, "temp_file open fail", NULL);
	limiter_len = ft_strlen(argv[2]) + 1;
	while (1)
	{
		line = get_next_line(STDIN_FILENO);
		if (line == NULL && errno != 0)
			exit_pipex(EXIT_FAILURE, errno, "get_next_line fail", NULL);
		ft_putstr_fd(line, in_fd);
		ft_free_str(&line);
		if (line == NULL || ft_strncmp(line, argv[2], limiter_len) == 0)
			break ;
	}
	close(in_fd);
	return ("temp_file");
}

int	here_doc(int argc, char *argv[], char **envp, char **paths)
{
	int	last_status;

	argv[2] = create_file_here_doc(argc, argv);
	last_status = iterative_pipex(argc - 2, argv + 2, envp, paths);
	unlink(argv[2]);
	return (last_status);
}
