/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_cmd_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migugar2 <migugar2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 19:19:36 by migugar2          #+#    #+#             */
/*   Updated: 2024/10/27 19:30:02 by migugar2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex_bonus.h"

int	**create_pipes(size_t n_pipes)
{
	int		**pipes;
	size_t	i;

	pipes = (int **)malloc(sizeof(int *) * n_pipes);
	if (pipes == NULL)
		return (NULL);
	i = 0;
	while (i < n_pipes)
	{
		pipes[i] = (int *)malloc(sizeof(int) * 2);
		if (pipes[i] == NULL)
			return (ft_free_int_matrix(pipes, i));
		i++;
	}
	return (pipes);
}

pid_t	execute_first(char *argv[], char **envp, char **paths, int pipe_fd[2])
{
	pid_t	pid;
	int		in_fd;
	char	**command;

	if (pipe(pipe_fd) == -1)
		exit_pipex(EXIT_FAILURE, errno, "pipe creation fail", NULL);
	pid = fork();
	if (pid == -1)
		exit_pipex(EXIT_FAILURE, errno, "fork fail", pipe_fd);
	if (pid == 0)
	{
		command = create_cmd(argv[1], paths);
		if (command == NULL)
			exit_pipex(127, errno, command[0], pipe_fd);
		in_fd = open(argv[0], O_RDONLY);
		if (in_fd == -1)
		{
			ft_free_str_matrix(command);
			exit_pipex(EXIT_FAILURE, errno, "infile open fail", pipe_fd);
		}
		close(pipe_fd[0]);
		execute_cmd(command, envp, in_fd, pipe_fd[1]);
	}
	close(pipe_fd[1]);
	return (pid);
}

// must read from the previous pipe ([0]) from read end ([0]) and write to the current pipe ([1]) to write end ([1])
pid_t	execute_middle(char *cmd_name, char **envp, char **paths, int **pipes)
{
	pid_t	pid;
	char	**command;

	if (pipe(pipes[1]) == -1)
		exit_pipex(EXIT_FAILURE, errno, "pipe creation fail", NULL);
	pid = fork();
	if (pid == -1)
		exit_pipex(EXIT_FAILURE, errno, "fork fail", pipes[0]);
	if (pid == 0)
	{
		command = create_cmd(cmd_name, paths);
		if (command == NULL)
			exit_pipex(127, errno, command, pipes[0]);
		close(pipes[0][1]);
		close(pipes[1][0]);
		execute_cmd(command, envp, pipes[0][0], pipes[1][1]);
	}
	close(pipes[0][0]);
	close(pipes[1][1]);
	return (pid);
}

pid_t	execute_last(char *argv[], char **envp, char **paths, int pipe_fd[2])
{
	pid_t	pid;
	int		out_fd;
	char	**command;

	out_fd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (out_fd == -1)
		exit_pipex(EXIT_FAILURE, errno, "outfile open fail", pipe_fd);
	pid = fork();
	if (pid == -1)
	{
		close(out_fd);
		exit_pipex(EXIT_FAILURE, errno, "outfile open fail", pipe_fd);
	}
	if (pid == 0)
	{
		command = create_cmd(argv[0], paths);
		if (command == NULL)
			exit_pipex(127, errno, command, pipe_fd[0]);
		close(pipe_fd[1]);
		execute_cmd(command, envp, pipe_fd[0], out_fd);
	}
	close(pipe_fd[0]);
	close(out_fd);
	return (pid);
}
