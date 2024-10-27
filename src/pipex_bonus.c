/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migugar2 <migugar2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 06:08:29 by migugar2          #+#    #+#             */
/*   Updated: 2024/10/28 00:01:20 by migugar2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex_bonus.h"

// ./pipex here_doc limiter cmd1 cmd2 cmdn outfile -> pipex: limiter cmd1 cmd2 cmdn outfile -> 5 arguments, 3 commands, 2 pipes
// ./pipex infile cmd1 cmd2 cmd3 cmd4 cmd5 cmdn outfile -> pipex: infile cmd1 cmd2 cmd3 cmd4 cmd5 cmdn outfile -> 8 arguments, 6 commands, 5 pipes
// total commands: argc - 4
// pipes: argc - 3
// commands in the middle: argc - 4 (index 0 is infile, so from index 1 to argc - 4)
// first command: index 1
// last command: index argc - 2

// ./pipex infile.txt "grep -v malloc" "grep -v PATH" "grep -v command" "grep -v return" "cat -e" /dev/stdout
// a

void	exit_pipex(int error, const char *message, int pipe_fd[2])
{
	if (pipe_fd != NULL)
	{
		close(pipe_fd[0]);
		close(pipe_fd[1]);
	}
	perror(message);
	exit(error);
}

int	pipex(int argc, char *argv[], char **envp, char **paths)
{
	int		**pipes;
	pid_t	last_pid;
	int		status;
	int		i;

	pipes = create_pipes(argc - 3);
	execute_first(argv, envp, paths, pipes[0]);
	i = 0;
	while (i < argc - 4)
	{
		execute_middle(argv[i + 2], envp, paths, pipes + i);
		i++;
	}
	last_pid = execute_last(argv + argc - 2, envp, paths, pipes[argc - 4]);
	i = 0;
	while (i < argc - 3)
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

	if (argc < 5)
	{
		errno = EINVAL;
		exit_pipex(22, "argc fail", NULL);
	}
	paths = get_paths(envp);
	if (paths == NULL)
		exit_pipex(EXIT_FAILURE, "path search failed", NULL);
	if (ft_strncmp(argv[1], "here_doc", 9) == 0)
	{
		if (argc < 6)
		{
			errno = EINVAL;
			exit_pipex(22, "argc fail", NULL);
		}
		last_status = here_doc(argc, argv, envp, paths);
	}
	else
		last_status = pipex(argc - 1, argv + 1, envp, paths); // !
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
