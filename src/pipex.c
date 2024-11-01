/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migugar2 <migugar2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 22:45:04 by migugar2          #+#    #+#             */
/*   Updated: 2024/11/01 13:11:09 by migugar2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

// errors
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
	char	**cmd;

	pid = fork();
	if (pid == -1)
		exit_pipex(EXIT_FAILURE, "fork error", pipe_fd);
	if (pid == 0)
	{
		cmd = create_cmd(argv[2], paths);
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
	return (pid);
}

pid_t	execute_second(char *argv[], char **envp, char **paths, int pipe_fd[2])
{
	pid_t	pid;
	int		out_fd;
	char	**command;

	out_fd = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
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
		command = create_cmd(argv[3], paths);
		if (command == NULL)
			exit_pipex(127, "command not found", pipe_fd);
		execute_cmd(command, envp, pipe_fd[0], out_fd);
	}
	close(pipe_fd[0]);
	close(out_fd);
	return (pid);
}

// pipex

int	pipex(char *argv[], char **envp, char **paths)
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
	exit_code = pipex(argv, envp, paths);
	ft_free_str_matrix(paths);
	return (exit_code);
}
