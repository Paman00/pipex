/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migugar2 <migugar2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 06:08:29 by migugar2          #+#    #+#             */
/*   Updated: 2024/10/27 19:29:46 by migugar2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex_bonus.h"

// ./pipex here_doc limiter cmd1 cmd2 cmdn outfile
// ./pipex infile cmd1 cmd2 cmdn outfile

void	exit_pipex(int error, int n_errno, const char *message, int pipe_fd[2])
{
	if (pipe_fd != NULL)
	{
		close(pipe_fd[0]);
		close(pipe_fd[1]);
	}
	errno = n_errno;
	perror(message);
	exit(error);
}

int	pipex(int argc, char *argv[], char **envp, char **paths)
{
	int		**pipes;
	pid_t	last_pid;
	int		status;
	size_t	i;

	pipes = create_pipes(argc - 3);
	execute_first(argv, envp, paths, pipes);
	i = 1;
	while (i < argc - 4)
	{
		execute_middle(argv[i + 1], envp, paths, pipes + i - 1);
		i++;
	}
	last_pid = execute_last(argv + i + 1, envp, paths, pipes + i - 1);
	i = 0;
	while (i < argc - 4)
	{
		wait(NULL);
		i++;
	}
	waitpid(last_pid, &status, 0);
	ft_free_int_matrix(pipes, argc - 3);
	return (status);
}

int	main(int argc, char *argv[], char **envp)
{
	char	**paths;
	int		last_status;
	pid_t	first;

	if (argc < 5)
		exit_pipex(22, EINVAL, "argc fail", NULL);
	paths = get_paths(envp);
	if (paths == NULL)
		exit_pipex(EXIT_FAILURE, errno, "path search failed", NULL);
	if (ft_strncmp(argv[1], "here_doc", 9) == 0)
	{
		if (argc < 6)
			exit_pipex(22, EINVAL, "argc fail", NULL);
		last_status = here_doc(argc, argv, envp, paths);
	}
	else
		last_status = iterative_pipex(argc - 1, argv + 1, envp, paths);
	ft_free_str_matrix(paths);
	if (WIFEXITED(last_status))
		return (WEXITSTATUS(last_status));
	return (EXIT_FAILURE);
}

/*
void	pipex(int argc, char *argv[], char **envp, int *last_status)
{
	char	**paths;
	int		i;

	paths = get_paths(envp);
	if (paths == NULL)
		exit_pipex(EXIT_FAILURE, errno, "path search failed", NULL);
	i = 1;
	while (i < argc - 2)
	{
		if (i == 1)
			execute_first(argv, envp, paths, pipe_fd);
		else if (i == argc - 2)
			execute_last(argv, envp, paths, pipe_fd);
		else
			execute_middle(argv, envp, paths, pipe_fd);
		i++;
	}
	// wait for each child process, also waitpid for last_pid
	// store the status of the last command in last_status
	// free paths
	while (i > 0)
	{
		if (i == argc - 2)
			waitpid(last_pid, last_status, 0);
		else
			wait(NULL);
		i--;
	}
}

void	here_doc(int argc, char *argv[], char **envp, int *last_status)
{
}

// must create multiple pipes, first cmd reads from infile (argv[1]), last cmd writes to outfile (argv[argc-1])
// the status of the last command must be stored in last_status
// main pid must open infile, create paths and free it, and execute all the commands, knowing if it is the first, middle or last command, and closing the pipes accordingly
int	main(int argc, char *argv[], char **envp)
{
	int		last_status;

	if (argc < 5)
		exit_pipex(EXIT_FAILURE, EINVAL, "argc fail", NULL);
	last_status = 0;
	if (ft_strncmp(argv[1], "here_doc", 9) == 0)
	{
		if (argc < 6)
			exit_pipex(EXIT_FAILURE, EINVAL, "argc fail", NULL);
		here_doc(argv, envp, paths, &last_status);
	}
	else
		pipex(argv, envp, paths, &last_status);
	ft_free_str_matrix(paths);
	if (WIFEXITED(last_status))
		return (WEXITSTATUS(last_status));
	return (EXIT_FAILURE);
}
*/

// With the get_next_line function, read from stdin until EOF or the delimiter is found with something like ft_strncmp(get_next_line(0), argv[2], ft_strlen(argv[2])) == 0, or return NULL
// I can read and while i'm reading, write to a temporal file, then close the file and open it, executing pipex with the temporal file as the infile
// Also i must close the file and delete it after the last command is executed
