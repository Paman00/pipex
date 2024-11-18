/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_exec_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migugar2 <migugar2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 21:24:33 by migugar2          #+#    #+#             */
/*   Updated: 2024/11/18 13:52:19 by migugar2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

int	execute_cmd(char *command_name, char **envp, int in_fd, int out_fd)
{
	char	**argv;
	char	*pathname;

	argv = ft_split(command_name, ' ');
	if (argv == NULL)
		return (seterrno(ENOMEM), -1);
	pathname = NULL;
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
		ft_close(&in_fd);
	}
	execve(pathname, argv, envp);
	return (ft_freestrarr(&argv), ft_freestr(&pathname), -1);
}

int	execute_first(char *cmd_name, char **envp, char *file_name)
{
	int		errnum;
	int		pipe_fd[2];
	pid_t	pid;
	int		in_fd;

	if (pipe(pipe_fd) == -1)
		return (-1);
	pid = fork();
	if (pid == -1)
		return (ft_close(&pipe_fd[0]), ft_close(&pipe_fd[1]), -1);
	if (pid == 0)
	{
		in_fd = open(file_name, O_RDONLY);
		ft_close(&pipe_fd[0]);
		if (in_fd == -1)
			exit(error_handler(0, file_name));
		if (execute_cmd(cmd_name, envp, in_fd, pipe_fd[1]) == -1)
		{
			errnum = errno;
			ft_close(&pipe_fd[1]);
			ft_close(&in_fd);
			exit_execute_error(errnum, cmd_name);
		}
	}
	return (ft_close(&pipe_fd[1]), pipe_fd[0]);
}

int	execute_middle(char *cmd_name, char **envp, int pipe_in)
{
	int		errnum;
	int		pipe_fd[2];
	pid_t	pid;

	if (pipe(pipe_fd) == -1)
		return (ft_close(&pipe_in), -1);
	pid = fork();
	if (pid == -1)
		return (ft_close(&pipe_in), ft_close(&pipe_fd[0]),
			ft_close(&pipe_fd[1]), -1);
	if (pid == 0)
	{
		if (execute_cmd(cmd_name, envp, pipe_in, pipe_fd[1]) == -1)
		{
			errnum = errno;
			ft_close(&pipe_in);
			ft_close(&pipe_fd[0]);
			ft_close(&pipe_fd[1]);
			exit_execute_error(errnum, cmd_name);
		}
	}
	return (ft_close(&pipe_in), ft_close(&pipe_fd[1]), pipe_fd[0]);
}

pid_t	execute_last(char *argv_last[], char **envp, int pipe_in, int oflag)
{
	pid_t	pid;
	int		out_fd;
	int		errnum;

	pid = fork();
	if (pid == -1)
		return (ft_close(&pipe_in), -1);
	if (pid == 0)
	{
		out_fd = open(argv_last[1], oflag, 0644);
		if (out_fd == -1)
		{
			ft_close(&pipe_in);
			exit(error_handler(0, argv_last[1]));
		}
		if (execute_cmd(argv_last[0], envp, pipe_in, out_fd) == -1)
		{
			errnum = errno;
			ft_close(&pipe_in);
			ft_close(&out_fd);
			exit_execute_error(errnum, argv_last[0]);
		}
	}
	return (ft_close(&pipe_in), pid);
}

void	wait_processes(int waitc)
{
	int	errnum;
	int	i;

	errnum = errno;
	i = 0;
	while (i < waitc)
	{
		if (wait(NULL) == -1)
			i = waitc;
		i++;
	}
	seterrno(errnum);
}
