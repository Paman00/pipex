/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migugar2 <migugar2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 19:16:38 by migugar2          #+#    #+#             */
/*   Updated: 2024/10/27 19:31:05 by migugar2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

pid_t	execute_first(char *argv[], char **envp, char **paths, int pipe_fd[2])
{
	pid_t	pid;
	int		in_fd;
	char	**cmd;

	pid = fork();
	if (pid == -1)
		exit_pipex(EXIT_FAILURE, "fork error", pipe_fd);
	if (pid == 0)
	{
		cmd = create_cmd(argv[2], paths);
		if (cmd == NULL)
			exit_pipex(127, "command not found", pipe_fd);
		in_fd = open(argv[1], O_RDONLY);
		if (in_fd == -1)
		{
			ft_free_str_matrix(cmd);
			exit_pipex(EXIT_FAILURE, "infile open fail", pipe_fd);
		}
		close(pipe_fd[0]);
		execute_cmd(cmd, envp, in_fd, pipe_fd[1]);
	}
	close(pipe_fd[1]);
	return (pid);
}

pid_t	execute_second(char *argv[], char **envp, char **paths, int pipe_fd[2])
{
	pid_t	pid;
	int		out_fd;
	char	**cmd;

	out_fd = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (out_fd == -1)
		exit_pipex(EXIT_FAILURE, "outfile open fail", pipe_fd);
	pid = fork();
	if (pid == -1)
	{
		close(out_fd);
		exit_pipex(EXIT_FAILURE, "outfile open fail", pipe_fd);
	}
	if (pid == 0)
	{
		cmd = create_cmd(argv[3], paths);
		if (cmd == NULL)
			exit_pipex(127, "command not found", pipe_fd);
		execute_cmd(cmd, envp, pipe_fd[0], out_fd);
	}
	close(pipe_fd[0]);
	close(out_fd);
	return (pid);
}
