/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migugar2 <migugar2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 22:45:04 by migugar2          #+#    #+#             */
/*   Updated: 2024/10/27 02:18:55 by migugar2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

// < archivo1 comando1 | comando2 > archivo2

// pipe() devuelve un arr[] de tipo int que tiene dos extremos, cada extremo
// es un fd, el [0] es para leer y el [1] es para escribir, lo que se escribe
// en el [1] se lee en el [0], es decir, se escribe en el pipe y se lee del pipe
// gracias a que el pipe es un buffer en memoria.

char	*get_cmd_path(char *cmd_name, char **paths)
{
	char	*cmd_path;
	size_t	i;

	i = 0;
	if (ft_strchr(cmd_name, '/') != NULL)
		return (ft_strdup(cmd_name));
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

char	**create_command(char *cmd, char **paths)
{
	char	**command;
	char	*cmd_path;

	command = ft_split(cmd, ' ');
	if (command == NULL)
		return (NULL); // I don't handle exactly this error, but it's ok
	cmd_path = get_cmd_path(command[0], paths);
	if (cmd_path == NULL)
		return (ft_free_str_matrix(command));
	free(command[0]);
	command[0] = cmd_path;
	return (command);
}

void	execute_cmd(char **command, char **envp, int in_fd, int out_fd)
{
	dup2(in_fd, 0);
	close(in_fd);
	dup2(out_fd, 1);
	close(out_fd);
	execve(command[0], command, envp);
	if (errno == ENOENT)
	{
		perror("command not found");
		exit(127);
	}
	if (errno == EACCES)
	{
		perror("permission denied");
		exit(126);
	}
	perror("execve fail");
	exit(EXIT_FAILURE);
}

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

void	execute_first_cmd(char *argv[], char **envp, char **paths, int pipe_fd[2])
{
	pid_t	pid;
	int		in_fd;
	char	**cmd;

	pid = fork();
	if (pid == -1)
		exit_pipex(EXIT_FAILURE, "fork error", pipe_fd);
	if (pid == 0)
	{
		cmd = create_command(argv[2], paths);
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
}

pid_t	execute_second_cmd(char *argv[], char **envp, char **paths, int pipe_fd[2])
{
	pid_t	pid;
	int		out_fd;
	char	**cmd;

	out_fd = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644); // TODO: probably must open the file in the child process ?, also francinette fails in test 11 if i make it in the child process
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
		cmd = create_command(argv[3], paths);
		if (cmd == NULL)
			exit_pipex(127, "command not found", pipe_fd);
		execute_cmd(cmd, envp, pipe_fd[0], out_fd);
		// close(pipe_fd[1]); // If i close before read it fails (idk why)
	}
	close(pipe_fd[0]);
	close(out_fd);
	return (pid);
}

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
			while (paths && paths[i] != NULL)
			{
				tmp = ft_strjoin(paths[i], "/");
				if (tmp == NULL)
					return (ft_free_str_matrix(paths));
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

int	pipex(char *argv[], char **envp, char **paths)
{
	int		pipe_fd[2];
	int		status;
	pid_t	pid_second;

	if (pipe(pipe_fd) == -1)
	{
		ft_free_str_matrix(paths);
		perror("pipe creation fail");
		exit(EXIT_FAILURE);
	}
	execute_first_cmd(argv, envp, paths, pipe_fd);
	pid_second = execute_second_cmd(argv, envp, paths, pipe_fd);
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
	exit_code = pipex(argv, envp, paths);
	ft_free_str_matrix(paths);
	return (exit_code);
}
