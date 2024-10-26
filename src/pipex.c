/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migugar2 <migugar2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 22:45:04 by migugar2          #+#    #+#             */
/*   Updated: 2024/10/25 22:35:29 by migugar2         ###   ########.fr       */
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
	while (paths[i] != NULL)
	{
		if (ft_strchr(cmd_name, '/') != NULL)
			return (ft_strdup(cmd_name));
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
		return (NULL);
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
	perror("execve failed");
	exit(EXIT_FAILURE);
}

void	execute_first_cmd(char *argv[], char **envp, char **paths, int pipe_fd[2])
{
	pid_t	pid;
	int		in_fd;
	char	**cmd;

	pid = fork();
	if (pid == -1)
	{
		close(pipe_fd[1]);
		close(pipe_fd[0]);
		perror("fork failed");
		exit(EXIT_FAILURE);
	}
	if (pid == 0)
	{
		cmd = create_command(argv[2], paths);
		if (cmd == NULL)
		{
			close(pipe_fd[0]);
			close(pipe_fd[1]);
			perror("cmd1 creation fail");
			exit(EXIT_FAILURE);
		}
		in_fd = open(argv[1], O_RDONLY);
		if (in_fd == -1)
		{
			ft_free_str_matrix(cmd);
			close(pipe_fd[1]);
			close(pipe_fd[0]);
			perror("infile open fail");
			exit(EXIT_FAILURE);
		}
		close(pipe_fd[0]);
		execute_cmd(cmd, envp, in_fd, pipe_fd[1]);
	}
	else
		close(pipe_fd[1]);
}

void	execute_second_cmd(char *argv[], char **envp, char **paths, int pipe_fd[2])
{
	pid_t	pid;
	int		out_fd;
	char	**cmd;

	out_fd = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644); // TODO: probably must open the file in the child process ?, also francinette fails in test 11 if i make it in the child process
	if (out_fd == -1)
	{
		close(pipe_fd[1]);
		close(pipe_fd[0]);
		perror("outfile open fail");
		exit(EXIT_FAILURE);
	}
	pid = fork();
	if (pid == -1)
	{
		close(pipe_fd[1]);
		close(pipe_fd[0]);
		close(out_fd);
		perror("fork failed");
		exit(EXIT_FAILURE);
	}
	if (pid == 0)
	{
		cmd = create_command(argv[3], paths);
		if (cmd == NULL)
		{
			ft_free_str_matrix(paths);
			close(pipe_fd[0]);
			close(pipe_fd[1]);
			perror("cmd2 creation fail");
			exit(EXIT_FAILURE);
		}
		execute_cmd(cmd, envp, pipe_fd[0], out_fd);
		// close(pipe_fd[1]); // If i close after read it fails (idk why)
	}
	else
	{
		close(pipe_fd[0]);
		close(out_fd);
	}
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

void	pipex(char *argv[], char **envp, char **paths)
{
	int		pipe_fd[2];
	int		status;

	if (pipe(pipe_fd) == -1)
	{
		ft_free_str_matrix(paths);
		perror("pipe creation fail");
		exit(EXIT_FAILURE);
	}
	execute_first_cmd(argv, envp, paths, pipe_fd);
	execute_second_cmd(argv, envp, paths, pipe_fd);
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	wait(&status);
	wait(&status);
}

int	main(int argc, char *argv[], char **envp)
{
	char	**paths;

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
	pipex(argv, envp, paths);
	ft_free_str_matrix(paths);
	return (0);
}

/*
void	pipex(char ***commands, char **envp, char *infile, char *outfile)
{
	int		pipe1[2];
	int		status;
	size_t	i;

	if (pipe(pipe1) == -1)
	{
		ft_free_str_trimatrix(commands);
		close(infile);
		close(outfile);
		perror("pipe creation fail");
		exit(EXIT_FAILURE);
	}
	execute_command(commands[0], envp, infile, pipe1[1]);
	close(pipe1[1]);
	execute_command(commands[1], envp, pipe1[0], outfile);
	close(pipe1[0]);
	i = 0;
	while (commands[i] != NULL)
	{
		if (wait(&status) == -1)
		{
			ft_free_str_trimatrix(commands);
			close(infile);
			close(outfile);
			perror("wait failed");
			exit(EXIT_FAILURE);
		}
		i++;
	}
}
char	*get_command_path(char *cmd_name, char **paths)
{
	char	*cmd_path;
	size_t	i;

	i = 0;
	while (paths[i] != NULL)
	{
		if (cmd_name[0] == '.' || ft_strchr(cmd_name, '/') != NULL)
		{
			if (access(cmd_name, F_OK | X_OK) == 0)
				return (ft_strdup(cmd_name));
			return (NULL);
		}
		cmd_path = ft_strjoin(paths[i], cmd_name);
		if (cmd_path == NULL)
			return (NULL); // malloc error
		if (access(cmd_path, F_OK | X_OK) == 0)
			return (cmd_path);
		free(cmd_path);
		i++;
	}
	return (NULL);
}

char	***create_commands(int argc, char *argv[], char **paths)
{
	char	***commands;
	char	*cmd_path;
	int		i;

	commands = (char ***)malloc(sizeof(char **) * (argc - 2));
	if (commands == NULL)
		return (NULL);
	i = 0;
	while (i + 2 < argc - 1)
	{
		commands[i] = ft_split(argv[i + 2], ' ');
		if (commands[i] == NULL)
			return (ft_free_str_trimatrix_r(commands, i));
		cmd_path = get_command_path(commands[i][0], paths);
		free(commands[i][0]);
		if (cmd_path == NULL)
			return (ft_free_str_trimatrix_r(commands, i));
		commands[i][0] = cmd_path;
		i++;
	}
	commands[i] = NULL;
	return (commands);
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

int	main(int argc, char *argv[], char **envp)
{
	char	**paths;
	int		infile;
	int		outfile;

	if (argc != 5)
	{
		perror("argc fail");
		exit(EXIT_FAILURE);
	}

	paths = get_paths(envp);
	if (paths == NULL)
	{
		perror("path fail search failed");
		exit(EXIT_FAILURE);
	}


	outfile = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (outfile == -1)
	{
		ft_free_str_matrix(paths);
		close(infile);
		perror("outfile open fail");
		exit(EXIT_FAILURE);
	}

	commands = create_commands(argc, argv, paths);
	if (commands == NULL)
	{
		ft_free_str_matrix(paths);
		close(infile);
		close(outfile);
		perror("commands creation fail");
		exit(EXIT_FAILURE);
	}

	ft_free_str_matrix(paths);
	pipex(commands, envp, infile, outfile);

	ft_free_str_trimatrix(commands);
	close(infile);
	close(outfile);
	return (0);
}
*/
