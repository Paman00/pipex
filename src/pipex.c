/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migugar2 <migugar2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 22:45:04 by migugar2          #+#    #+#             */
/*   Updated: 2024/10/27 10:15:59 by migugar2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

void	exit_pipex(int error_code, const char *message, int pipe_fd[2])
{
	if (pipe_fd != NULL)
	{
		close(pipe_fd[0]);
		close(pipe_fd[1]);
	}
	perror(message);
	exit(error_code);
}

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

int	pipex_simple(char *argv[], char **envp, char **paths)
{
	int		pipe_fd[2];
	int		status;
	pid_t	pid_second;

	if (pipe(pipe_fd) == -1)
	{
		ft_free_str_matrix(paths);
		exit_pipex(EXIT_FAILURE, "pipe creation fail", NULL);
	}
	execute_first(argv, envp, paths, pipe_fd);
	pid_second = execute_second(argv, envp, paths, pipe_fd);
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	wait(&status);
	waitpid(pid_second, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (EXIT_FAILURE);
}

int	main(int argc, char *argv[], char **envp)
{
	char	**paths;
	int		exit_code;

	if (argc != 5)
	{
		perror("argc fail");
		exit(EXIT_FAILURE);
	}
	paths = get_paths(envp);
	if (paths == NULL)
	{
		perror("path search failed");
		exit(EXIT_FAILURE);
	}
	exit_code = pipex_simple(argv, envp, paths);
	ft_free_str_matrix(paths);
	return (exit_code);
}
