/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migugar2 <migugar2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 06:08:29 by migugar2          #+#    #+#             */
/*   Updated: 2024/11/15 15:08:27 by migugar2         ###   ########.fr       */
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
// TODO: change and adapt every error function to libft functions
int	set_errno(int errnum)
{
	if (errno == 0)
		errno = errnum;
	return (errnum);
}

/*
char	*str_error(int nerrno)
{
	if (nerrno == -1)
		return ("command not found");
	return (strerror(nerrno));
}

int	p_nerror(const char *arg, int nerror)
{
	errno = nerror;
	return (print_error(arg));
}
*/

int	cmd_not_found(const char *command)
{
	if (command == NULL)
		ft_printf_error("pipex: command not found\n");
	else
		ft_printf_error("pipex: command not found: %s\n", command);
	return (127);
}

int	perror_pipex(int errnum, const char *arg)
{
	ft_printf_error("pipex: ");
	ft_perror(errnum, arg);
	return (errnum);
}

int	perrno_pipex(const char *arg)
{
	int	errnum;

	errnum = errno;
	ft_printf_error("pipex: ");
	ft_perror(errnum, arg);
	return (errnum);
}

int	exit_forked(const char *arg, int pipe_fd[2])
{
	int	errnum;

	errnum = errno;
	if (pipe_fd != NULL)
	{
		close(pipe_fd[0]);
		close(pipe_fd[1]);
	}
	if (errnum == ENOENT)
		exit(cmd_not_found(arg));
	if (errnum == EACCES)
	{
		perrno_pipex(arg);
		exit(126);
	}
	perrno_pipex(arg);
	exit(EXIT_FAILURE);
	return (errnum);
}

int	exit_pipex(int error, const char *message, int pipe_fd[2])
{
	if (pipe_fd != NULL)
	{
		close(pipe_fd[0]);
		close(pipe_fd[1]);
	}
	perror(message);
	exit(error);
	return (error);
}

int	exit_not_pipex(int error, const char *message)
{
	perror(message);
	exit(error);
	return (error);
}

// paths and command creation
char	*get_cmd_path(char *cmd_name, char **paths)
{
	char	*cmd_path;
	size_t	i;

	if (cmd_name == NULL)
		return (NULL);
	if (ft_strchr(cmd_name, '/') != NULL)
	{
		cmd_path = ft_strdup(cmd_name);
		if (cmd_path == NULL)
			return (set_errno(ENOMEM), NULL);
		return (cmd_path);
	}
	i = 0;
	while (paths[i] != NULL)
	{
		cmd_path = ft_strjoin(paths[i], cmd_name);
		if (cmd_path == NULL)
			return (set_errno(ENOMEM), NULL);
		if (access(cmd_path, X_OK) == 0)
			return (cmd_path);
		ft_free_str(&cmd_path);
		errno = 0;
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
		return (set_errno(ENOMEM), NULL);
	cmd_path = get_cmd_path(command[0], paths);
	if (cmd_path == NULL)
		return (ft_free_str_matrix(command), NULL);
	free(command[0]);
	command[0] = cmd_path;
	return (command);
}

// command execution
void	execute_cmd(char **command, char **envp, int in_fd, int out_fd)
{
	int	errnum;

	if (dup2(in_fd, 0) == -1)
	{
		errnum = errno;
		close(in_fd);
		close(out_fd);
		ft_free_str_matrix(command);
		errno = errnum;
		exit_forked(NULL, NULL);
	}
	close(in_fd);
	if (dup2(out_fd, 1) == -1)
	{
		errnum = errno;
		close(out_fd);
		ft_free_str_matrix(command);
		errno = errnum;
		exit_forked(NULL, NULL);
	}
	close(out_fd);
	execve(command[0], command, envp);
	exit(exit_forked(command[0], NULL));
}

pid_t	execute_first(char *argv[], char **envp, char **paths, int pipe_fd[2])
{
	pid_t	pid;
	int		in_fd;
	char	**command;

	// printf("execute_first argv[0]: %s\n", argv[0]);
	// printf("execute_first argv[1]: %s\n", argv[1]);
	if (pipe(pipe_fd) == -1)
		return (print_error(NULL), -1);
	pid = fork();
	if (pid == -1)
		return (print_error(NULL), close(pipe_fd[0]), close(pipe_fd[1]), -1);
	if (pid == 0)
	{
		command = create_cmd(argv[1], paths);
		if (command == NULL)
			exit_forked(argv[1], pipe_fd);
		in_fd = open(argv[0], O_RDONLY);
		if (in_fd == -1)
		{
			ft_free_str_matrix(command);
			exit_forked(EXIT_FAILURE, "infile open fail", pipe_fd);
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

	// printf("execute_middle cmd_name: %s\n", cmd_name);
	if (pipe(pipes[1]) == -1)
		return (print_error(NULL), -1);
	pid = fork();
	if (pid == -1)
		return (print_error(NULL), close(pipes[1][0]), close(pipes[1][1]), -1);
	if (pid == 0)
	{
		command = create_cmd(cmd_name, paths);
		if (command == NULL)
			exit_forked(127, cmd_name, pipes[0]);
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

	// printf("execute_last argv[0]: %s\n", argv[0]);
	// printf("execute_last argv[1]: %s\n", argv[1]);
	pid = fork();
	if (pid == -1)
		return (print_error(NULL), -1);
	if (pid == 0)
	{
		out_fd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (out_fd == -1)
			exit_forked(EXIT_FAILURE, "outfile open fail", pipe_fd);
		printf("errno: %d\n", errno);
		command = create_cmd(argv[0], paths);
		printf("errno: %d\n", errno);
		if (command == NULL)
		{
			printf("1 out_fd: %d\n", out_fd);
			printf("errno: %d\n", errno);
			// close(out_fd);
			printf("errno: %d\n", errno);
			exit_forked(127, argv[0], pipe_fd);
		}
		printf("2 out_fd: %d\n", out_fd);
		execute_cmd(command, envp, pipe_fd[0], out_fd);
	}
	close(pipe_fd[0]);
	return (pid);
}

// pipes creation
int	**create_pipes(int n_pipes)
{
	int		**pipes;
	int		i;

	pipes = (int **)malloc(sizeof(int *) * n_pipes);
	if (pipes == NULL)
		return (set_errno(ENOMEM), NULL);
	i = 0;
	while (i < n_pipes)
	{
		pipes[i] = (int *)malloc(sizeof(int) * 2);
		if (pipes[i] == NULL)
			return (set_errno(ENOMEM), ft_free_int_matrix(pipes, i));
		i++;
	}
	return (pipes);
}

// here_doc
pid_t	execute_heredoc(char *argv[], char **envp, char **paths, int pipe_fd[2])
{
	pid_t	pid;
	int		out_fd;
	char	**command;

	pid = fork();
	if (pid == -1)
		return (print_error(NULL), close(pipe_fd[0]), close(pipe_fd[1]), -1);
	if (pid == 0)
	{
		out_fd = open(argv[1], O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (out_fd == -1)
			exit_forked(EXIT_FAILURE, "outfile open fail", pipe_fd);
		command = create_cmd(argv[0], paths);
		if (command == NULL)
		{
			close(out_fd);
			exit_forked(127, argv[0], pipe_fd);
		}
		execute_cmd(command, envp, pipe_fd[0], out_fd);
	}
	close(pipe_fd[0]);
	return (pid);
}

char	*create_temp_name(void)
{
	char	*temp_name;
	size_t	i;

	temp_name = ft_strdup("temp_file");
	if (temp_name == NULL)
		return (set_errno(ENOMEM), NULL);
	if (access(temp_name, F_OK) == -1)
		return (temp_name);
	i = 0;
	while (i < 0)
	{
		free(temp_name);
		temp_name = ft_strjoin("temp_file_", ft_itoa(i));
		if (temp_name == NULL)
			return (set_errno(ENOMEM), NULL);
		if (access(temp_name, F_OK) == -1)
			return (temp_name);
		i++;
	}
	free(temp_name);
	return (set_errno(EEXIST), NULL);
}

int	**free_pipes(int **pipes, int opens, int n_pipes)
{
	int	i;

	i = 0;
	while (i < opens)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
		i++;
	}
	return (ft_free_int_matrix(pipes, n_pipes));
}

int	here_doc_pipex(int argc, char *argv[], char **envp, char **paths)
{
	int		**pipes;
	pid_t	last_pid;
	int		status;
	int		i;
	int		cmdc;

	cmdc = argc - 3;
	pipes = create_pipes(cmdc - 1);
	if (pipes == NULL)
		return (print_error(NULL), -1);
	if (execute_first(argv + 2, envp, paths, pipes[0]) == -1)
		return (ft_free_int_matrix(pipes, cmdc - 1), -1);
	i = 0;
	while (i < cmdc - 2)
	{
		if (execute_middle(argv[i + 3], envp, paths, pipes + i) == -1)
			return (free_pipes(pipes, i, cmdc - 1), -1);
		i++;
	}
	last_pid = execute_heredoc(argv + argc - 2, envp, paths, pipes[cmdc - 2]);
	if (last_pid == -1)
		return (free_pipes(pipes, i, cmdc - 1), -1);
	i = 0;
	while (i < cmdc - 1)
	{
		wait(NULL);
		i++;
	}
	waitpid(last_pid, &status, 0);
	return (ft_free_int_matrix(pipes, cmdc - 1), status);
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
		return (print_error(NULL), -1);
	if (execute_first(argv + 1, envp, paths, pipes[0]) == -1)
		return (ft_free_int_matrix(pipes, cmdc - 1), -1);
	i = 0;
	while (i < cmdc - 2)
	{
		if (execute_middle(argv[i + 3], envp, paths, pipes + i) == -1)
			return (ft_free_int_matrix(pipes, cmdc - 1), -1);
		i++;
	}
	last_pid = execute_last(argv + argc - 2, envp, paths, pipes[cmdc - 2]);
	if (last_pid == -1)
		return (ft_free_int_matrix(pipes, cmdc - 1), -1);
	i = 0;
	while (i < cmdc - 1)
	{
		wait(NULL);
		i++;
	}
	waitpid(last_pid, &status, 0);
	return (ft_free_int_matrix(pipes, cmdc - 1), status);
}

char	*create_file_here_doc(char *limiter)
{
	int		in_fd;
	char	*line;
	size_t	limiter_len;
	char	*file_name;

	file_name = create_temp_name();
	if (file_name == NULL)
		return (NULL);
	in_fd = open(file_name, O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (in_fd == -1)
		return (ft_free_str(&file_name));
	limiter_len = ft_strlen(limiter);
	while (1)
	{
		line = get_next_line(STDIN_FILENO); // TODO: change get_next_line to set errno when malloc fails
		if (line == NULL && errno != 0)
			return (close(in_fd), ft_free_str(&file_name));
		if (line == NULL || (ft_strlen(line) - 1 == limiter_len
				&& ft_strncmp(line, limiter, limiter_len) == 0))
			break ;
		ft_putstr_fd(line, in_fd);
		ft_free_str(&line);
	}
	return (close(in_fd), ft_free_str(&line), file_name);
}

int	here_doc(int argc, char *argv[], char **envp, char **paths)
{
	char	*new_file;
	int		last_status;

	new_file = create_file_here_doc(argv[2]);
	if (new_file == NULL)
		return (print_error(NULL), -1);
	argv[2] = new_file;
	last_status = here_doc_pipex(argc, argv, envp, paths);
	unlink(argv[2]);
	ft_free_str(&argv[2]);
	return (last_status);
}

char	**get_paths(char ***paths, char **envp)
{
	char	*tmp;
	size_t	i;

	i = 0;
	while (envp != NULL && envp[i] != NULL)
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
		{
			*paths = ft_split(envp[i] + 5, ':');
			if (*paths == NULL)
				return (set_errno(ENOMEM), NULL);
			i = 0;
			while ((*paths)[i] != NULL)
			{
				tmp = ft_strjoin((*paths)[i], "/");
				if (tmp == NULL)
					return (set_errno(ENOMEM), ft_free_str_matrix_r(*paths, i));
				free((*paths)[i]);
				(*paths)[i++] = tmp;
			}
			return (*paths);
		}
		i++;
	}
	return (set_errno(ENOENT), NULL);
}


int	main(int argc, char *argv[], char **envp)
{
	char	**paths;
	int		last_status;

	errno = 0;
	if (argc < 5)
		return (p_nerror("argc fail", EINVAL), EXIT_FAILURE);
	if (get_paths(&paths, envp) == NULL)
		return (print_error(NULL), EXIT_FAILURE);
	if (ft_strncmp(argv[1], "here_doc", 9) == 0)
	{
		if (argc < 6)
			return (p_nerror("argc fail", EINVAL), EXIT_FAILURE);
		last_status = here_doc(argc, argv, envp, paths);
	}
	else
		last_status = pipex(argc, argv, envp, paths);
	ft_free_str_matrix(paths);
	if (last_status == -1 && errno == 0)
		return (p_nerror(NULL, 0), EXIT_FAILURE);
	if (last_status == -1)
		return (EXIT_FAILURE);
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
