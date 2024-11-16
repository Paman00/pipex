/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_exec.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migugar2 <migugar2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 17:35:22 by migugar2          #+#    #+#             */
/*   Updated: 2024/11/16 20:19:26 by migugar2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	execute_cmd(char *command_name, char **envp, int in_fd, int out_fd)
{
	char	**argv;
	char	*pathname;

	argv = ft_split(command_name, ' ');
	if (argv == NULL)
		return (seterrno(ENOMEM), -1);
	if (get_cmd_path(&pathname, argv[0], envp) == -1)
		return (ft_freestrarr(&argv), ft_freestr(&pathname), -1);
	if (in_fd != STDIN_FILENO)
	{
		dup2(in_fd, STDIN_FILENO);
		ft_close(&in_fd);
	}
	if (out_fd != STDOUT_FILENO)
	{
		dup2(out_fd, STDOUT_FILENO);
		ft_close(&out_fd);
	}
	execve(pathname, argv, envp);
	return (ft_freestrarr(&argv), ft_freestr(&pathname), -1);
}

pid_t	execute_first(char *argv[], char **envp, int pipe_fd[2])
{
	pid_t	pid;
	int		in_fd;
	int		errnum;

	pid = fork();
	if (pid == -1)
		exit(error_handler(0, NULL));
	if (pid == 0)
	{
		in_fd = open(argv[1], O_RDONLY);
		if (in_fd == -1)
			exit(error_handler(0, argv[1]));
		ft_close(&pipe_fd[0]);
		if (execute_cmd(argv[2], envp, in_fd, pipe_fd[1]) == -1)
		{
			errnum = errno;
			ft_close(&in_fd);
			exit_execute_error(errnum, argv[2]);
		}
	}
	ft_close(&pipe_fd[1]);
	return (pid);
}

pid_t	execute_second(char *argv[], char **envp, int pipe_fd[2])
{
	pid_t	pid;
	int		out_fd;
	int		errnum;

	pid = fork();
	if (pid == -1)
		exit(error_handler(0, NULL));
	if (pid == 0)
	{
		out_fd = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (out_fd == -1)
			exit(error_handler(0, argv[4]));
		if (execute_cmd(argv[3], envp, pipe_fd[0], out_fd) == -1)
		{
			errnum = errno;
			ft_close(&pipe_fd[0]);
			ft_close(&out_fd);
			exit_execute_error(errnum, argv[3]);
		}
	}
	ft_close(&pipe_fd[0]);
	ft_close(&out_fd);
	return (pid);
}
