/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migugar2 <migugar2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 06:08:29 by migugar2          #+#    #+#             */
/*   Updated: 2024/11/18 11:18:25 by migugar2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

int	pipex(int argc, char *argv[], char **envp, int oflag)
{
	pid_t	last_pid;
	int		status;
	int		cmdc;
	int		pipe_in;
	int		i;

	cmdc = argc - 3;
	i = 0;
	while (i < cmdc - 1)
	{
		if (i == 0)
			pipe_in = execute_first(argv[2], envp, argv[1]);
		else
			pipe_in = execute_middle(argv[i + 2], envp, pipe_in);
		if (pipe_in == -1)
			return (wait_processes(i), -1);
		i++;
	}
	last_pid = execute_last(argv + argc - 2, envp, pipe_in, oflag);
	if (last_pid == -1)
		return (wait_processes(i), -1);
	waitpid(last_pid, &status, 0);
	wait_processes(i);
	return (status);
}

int	main(int argc, char *argv[], char **envp)
{
	int	last_status;

	if (argc == 1)
		exit(error_handler(1, argv[0]));
	else if (ft_strncmp(argv[1], "here_doc", 9) == 0)
	{
		if (argc < 6)
			exit(error_handler(4, argv[0]));
		last_status = here_doc(argc, argv, envp);
	}
	else
	{
		if (argc < 5)
			exit(error_handler(1, argv[0]));
		last_status = pipex(argc, argv, envp, O_CREAT | O_WRONLY | O_TRUNC);
	}
	if (last_status == -1)
		return (error_handler(0, NULL));
	else if (WIFEXITED(last_status))
		return (WEXITSTATUS(last_status));
	else if (WIFSIGNALED(last_status))
		return (WTERMSIG(last_status) + 128);
	return (EXIT_FAILURE);
}
