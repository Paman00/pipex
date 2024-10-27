/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migugar2 <migugar2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 22:45:04 by migugar2          #+#    #+#             */
/*   Updated: 2024/10/27 19:31:37 by migugar2         ###   ########.fr       */
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
