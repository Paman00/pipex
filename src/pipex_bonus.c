/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migugar2 <migugar2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 06:08:29 by migugar2          #+#    #+#             */
/*   Updated: 2024/11/01 13:11:02 by migugar2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex_bonus.h"

// ./pipex infile.txt "grep -v malloc" "grep -v PATH" "grep -v command" "grep -v return" "cat -e" /dev/stdout
// main:
// argc = 8, commands = 5
// in pipex function:
// infile.txt "grep -v malloc" "grep -v PATH" "grep -v command" "grep -v return" "cat -e" /dev/stdout
// argc = 7, commands = 5, first_command=1 (1), last_command=6 (1), middle_commands=2-5 (3), pipes=4

// ./pipex here_doc limiter "grep -v malloc" "grep -v PATH" "grep -v command" "grep -v return" "cat -e" /dev/stdout

// errors
int	set_errno(int error)
{
	if (errno == 0)
		errno = error;
	return (error);
}

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

void	exit_error(int error_code, const char *message, char **paths)
{
	ft_free_str_matrix(paths);
	perror(message);
	exit(error_code);
}

// paths and command creation
char	**get_paths(char **envp)
{
	char	**paths;
	char	*tmp;
	size_t	i;

	i = 0;
	while (envp != NULL && envp[i] != NULL)
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
		{
			paths = ft_split(envp[i] + 5, ':');
			i = 0;
			while (paths != NULL && paths[i] != NULL)
			{
				tmp = ft_strjoin(paths[i], "/");
				if (tmp == NULL)
					return (ft_free_str_matrix_r(paths, i));
				free(paths[i]);
				paths[i] = tmp;
				i++;
			}
			return (paths);
		}
		i++;
	}
	return (NULL);
}

char	*get_cmd_path(char *cmd_name, char **paths)
{
	char	*cmd_path;
	size_t	i;

	if (cmd_name == NULL)
		return (NULL);
	if (ft_strchr(cmd_name, '/') != NULL)
		return (ft_strdup(cmd_name));
	i = 0;
	while (paths[i] != NULL)
	{
		cmd_path = ft_strjoin(paths[i], cmd_name);
		if (cmd_path == NULL)
			return (NULL);
		if (access(cmd_path, X_OK) == 0)
			return (cmd_path);
		free(cmd_path);
		i++;
	}
	return (NULL);
}

char	**create_cmd(char *cmd_name, char **paths)
{
	char	**command;
	char	*cmd_path;

	command = ft_split(cmd_name, ' ');
	if (command == NULL)
		return (NULL);
	cmd_path = get_cmd_path(command[0], paths);
	if (cmd_path == NULL)
		return (ft_free_str_matrix(command));
	free(command[0]);
	command[0] = cmd_path;
	return (command);
}

// command execution
void	execute_cmd(char **command, char **envp, int in_fd, int out_fd)
{
	if (in_fd != 0)
	{
		dup2(in_fd, 0);
		close(in_fd);
	}
	if (out_fd != 1)
	{
		dup2(out_fd, 1);
		close(out_fd);
	}
	execve(command[0], command, envp);
	ft_free_str_matrix(command);
	if (errno == ENOENT)
		exit_error(127, "command not found", NULL);
	if (errno == EACCES)
		exit_error(126, "permission denied", NULL);
	perror("execve fail");
	exit(EXIT_FAILURE);
}

pid_t	execute_first(char *argv[], char **envp, char **paths, int pipe_fd[2])
{
	pid_t	pid;
	int		in_fd;
	char	**command;

	if (pipe(pipe_fd) == -1)
		exit_pipex(EXIT_FAILURE, "pipe creation fail", NULL);
	pid = fork();
	if (pid == -1)
		exit_pipex(EXIT_FAILURE, "fork fail", pipe_fd);
	if (pid == 0)
	{
		command = create_cmd(argv[1], paths);
		if (command == NULL)
			exit_pipex(127, argv[1], pipe_fd);
		in_fd = open(argv[0], O_RDONLY);
		if (in_fd == -1)
		{
			ft_free_str_matrix(command);
			exit_pipex(EXIT_FAILURE, "infile open fail", pipe_fd);
		}
		close(pipe_fd[0]);
		execute_cmd(command, envp, in_fd, pipe_fd[1]);
	}
	close(pipe_fd[1]);
	return (pid);
}

pid_t	execute_middle(char *cmd_name, char **envp, char **paths, int **pipes)
{
	pid_t	pid;
	char	**command;

	if (pipe(pipes[1]) == -1)
		exit_pipex(EXIT_FAILURE, "pipe creation fail", NULL);
	pid = fork();
	if (pid == -1)
		exit_pipex(EXIT_FAILURE, "fork fail", pipes[0]);
	if (pid == 0)
	{
		command = create_cmd(cmd_name, paths);
		if (command == NULL)
			exit_pipex(127, cmd_name, pipes[0]);
		execute_cmd(command, envp, pipes[0][0], pipes[1][1]);
	}
	close(pipes[1][1]);
	return (pid);
}

pid_t	execute_last(char *argv[], char **envp, char **paths, int pipe_fd[2])
{
	pid_t	pid;
	int		out_fd;
	char	**command;

	pid = fork();
	if (pid == -1)
		exit_pipex(EXIT_FAILURE, "outfile open fail", pipe_fd);
	if (pid == 0)
	{
		out_fd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (out_fd == -1)
			exit_pipex(EXIT_FAILURE, "outfile open fail", pipe_fd);
		command = create_cmd(argv[0], paths);
		if (command == NULL)
		{
			close(out_fd);
			exit_pipex(127, argv[0], pipe_fd);
		}
		// close(pipe_fd[1]);
		execute_cmd(command, envp, pipe_fd[0], out_fd);
	}
	close(pipe_fd[0]);
	return (pid);
}

// here_doc
pid_t	execute_heredoc(char *argv[], char **envp, char **paths, int pipe_fd[2])
{
	pid_t	pid;
	int		out_fd;
	char	**command;

	pid = fork();
	if (pid == -1)
		exit_pipex(EXIT_FAILURE, "outfile open fail", pipe_fd);
	if (pid == 0)
	{
		out_fd = open(argv[1], O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (out_fd == -1)
			exit_pipex(EXIT_FAILURE, "outfile open fail", pipe_fd);
		command = create_cmd(argv[0], paths);
		if (command == NULL)
		{
			close(out_fd);
			exit_pipex(127, argv[0], pipe_fd);
		}
		execute_cmd(command, envp, pipe_fd[0], out_fd);
	}
	close(pipe_fd[0]);
	return (pid);
}

char	*create_file_here_doc(char *limiter)
{
	int		in_fd;
	char	*line;
	size_t	limiter_len;
	char	*file_name;

	// file_name = create_temp_name();
	file_name = ft_strdup("temp_file");
	if (file_name == NULL)
		exit_pipex(EXIT_FAILURE, "temp_file name creation fail", NULL); // TODO: return NULL after setting errno to ENOMEM or print error message for free elements in other functions
	in_fd = open(file_name, O_CREAT | O_RDWR | O_TRUNC, 0644); // TODO: generate name with malloc checking if name is already taken
	if (in_fd == -1)
		exit_pipex(EXIT_FAILURE, "temp_file open fail", NULL); // TODO: free file_name, set errno to EACCES or EEXIST and return NULL
	limiter_len = ft_strlen(limiter);
	while (1)
	{
		line = get_next_line(STDIN_FILENO);
		if (line == NULL && errno != 0)
			exit_pipex(EXIT_FAILURE, "get_next_line fail", NULL); // TODO: free file_name, close in_fd, return NULL, errno is already set
		if (line == NULL)
			break ;
		if (ft_strlen(line) - 1 == limiter_len
			&& ft_strncmp(line, limiter, limiter_len) == 0)
			break ;
		ft_putstr_fd(line, in_fd);
		ft_free_str(&line);
	}
	return (ft_free_str(&line), close(in_fd), file_name);
}

int	here_doc_pipex(int argc, char *argv[], char **envp, char **paths)
{
	int		**pipes;
	pid_t	last_pid;
	int		status;
	int		i;
	int		cmdc;

	cmdc = argc - 4;
	pipes = create_pipes(cmdc - 1);
	if (pipes == NULL)
		exit_pipex(EXIT_FAILURE, "pipe creation fail", NULL);
	execute_first(argv + 2, envp, paths, pipes[0]);
	i = 0;
	while (i < cmdc - 2)
	{
		execute_middle(argv[i + 3], envp, paths, pipes + i);
		i++;
	}
	last_pid = execute_heredoc(argv + argc - 2, envp, paths, pipes[cmdc - 2]);
	i = 0;
	while (i < cmdc - 1)
	{
		wait(NULL);
		i++;
	}
	waitpid(last_pid, &status, 0);
	return (ft_free_int_matrix(pipes, cmdc - 1), status);
}

int	here_doc(int argc, char *argv[], char **envp, char **paths)
{
	int		last_status;

	argv[2] = create_file_here_doc(argv[2]);
	last_status = here_doc_pipex(argc, argv, envp, paths);
	unlink(argv[2]);
	ft_free_str(&argv[2]);
	return (last_status);
}

// pipes creation
int	**create_pipes(int n_pipes)
{
	int		**pipes;
	int		i;

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

// pipex

int	pipex(int argc, char *argv[], char **envp, char **paths)
{
	int		**pipes;
	pid_t	last_pid;
	int		status;
	int		i;
	int		cmdc;

	cmdc = argc - 3;
	pipes = create_pipes(cmdc - 1);
	if (pipes == NULL)
		exit_pipex(EXIT_FAILURE, "pipe creation fail", NULL); // TODO: set errno to ENOMEM and exit_error for free paths
	execute_first(argv + 1, envp, paths, pipes[0]);
	i = 0;
	while (i < cmdc - 2)
	{
		execute_middle(argv[i + 3], envp, paths, pipes + i);
		i++;
	}
	last_pid = execute_last(argv + argc - 2, envp, paths, pipes[cmdc - 2]);
	i = 0;
	while (i < cmdc - 1)
	{
		wait(NULL);
		i++;
	}
	waitpid(last_pid, &status, 0);
	return (ft_free_int_matrix(pipes, cmdc - 1), status);
}

int	main(int argc, char *argv[], char **envp)
{
	char	**paths;
	int		last_status;

	errno = 0;
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
		last_status = pipex(argc, argv, envp, paths);
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
