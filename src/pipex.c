/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migugar2 <migugar2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 22:45:04 by migugar2          #+#    #+#             */
/*   Updated: 2024/11/16 19:54:23 by migugar2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	pipex(char *argv[], char **envp)
{
	int		pipe_fd[2];
	int		status;
	pid_t	pid_second;

	if (pipe(pipe_fd) == -1)
		exit(error_handler(0, NULL));
	execute_first(argv, envp, pipe_fd);
	pid_second = execute_second(argv, envp, pipe_fd);
	ft_close(&pipe_fd[0]);
	ft_close(&pipe_fd[1]);
	wait(NULL);
	waitpid(pid_second, &status, 0);
	return (status);
}

int	main(int argc, char *argv[], char **envp)
{
	int	status_last;

	if (argc != 5)
		exit(error_handler(1, argv[0]));
	status_last = pipex(argv, envp);
	if (WIFEXITED(status_last))
		return (WEXITSTATUS(status_last));
	return (EXIT_FAILURE);
}
