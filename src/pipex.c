/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migugar2 <migugar2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 22:45:04 by migugar2          #+#    #+#             */
/*   Updated: 2024/10/24 21:19:06 by migugar2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

// < archivo1 comando1 | comando2 > archivo2

// pipe() devuelve un arr[] de tipo int que tiene dos extremos, cada extremo
// es un fd, el [0] es para leer y el [1] es para escribir, lo que se escribe
// en el [1] se lee en el [0], es decir, se escribe en el pipe y se lee del pipe
// gracias a que el pipe es un buffer en memoria.

pid_t	execute_command(char **command, char **envp, int in_fd, int out_fd)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("fork failed");
		exit(EXIT_FAILURE);
	}
	if (pid == 0)
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
		perror("execve failed");
		exit(EXIT_FAILURE);
	}
	return (pid);
}

void	pipex(char ***commands, char **envp, int infile, int outfile)
{
	int		pipe1[2];

	if (pipe(pipe1) == -1)
	{
		close(infile);
		close(outfile);
		perror("pipe creation fail");
		exit(EXIT_FAILURE);
	}
	execute_command(commands[0], envp, infile, pipe1[1]);
	close(pipe1[1]);
	free(commands[0]); // TODO: free the command struct
	execute_command(commands[1], envp, pipe1[0], outfile);
	close(pipe1[0]);
	free(commands[1]); // TODO: free the command struct
	wait(NULL);
	wait(NULL);
/*
	int		pipe1[2];
	size_t	i;
	int		infile;
	int		outfile;
	char	**cmd1;
	char	**cmd2;

	cmd1 = ft_split(argv[2], ' ');
	if (cmd1 == NULL)
	{
		close(infile);
		close(outfile);
		perror("malloc failed");
		exit(EXIT_FAILURE);
	}

	cmd2 = ft_split(argv[3], ' ');
	if (cmd2 == NULL)
	{
		close(infile);
		close(outfile);
		free(cmd1); // free must be for each position of the cmd1 arr, like an free_matrix function
		perror("malloc failed");
		exit(EXIT_FAILURE);
	}

	if (pipe(pipe1) == -1)
	{
		close(infile);
		close(outfile);
		perror("pipe creation fail");
		exit(EXIT_FAILURE);
	}

	cmd1 = ft_split(argv[2], ' '); // char *cmd1[] = { "/bin/ls", "-l", NULL };
	if (cmd1 == NULL)
	{
		close(pipe1[0]);
		close(pipe1[1]);
		close(infile);
		close(outfile);
		perror("malloc fail");
		exit(EXIT_FAILURE);
	}

	execute_command(cmd1, infile, pipe1[1]);
	close(pipe1[1]);
	free(cmd1);

	cmd2 = ft_split(argv[3], ' '); // char *cmd2[] = { "/bin/grep", ".c", NULL };
	if (cmd2 == NULL)
	{
		close(pipe1[0]);
		close(infile);
		close(outfile);
		perror("malloc fail");
		exit(EXIT_FAILURE);
	}
	execute_command(cmd2, pipe1[0], outfile);

	close(pipe1[0]);
	close(outfile);
	free(cmd2);

	i = 0;
	wait(NULL);
	wait(NULL);
	*/
}

char	*get_command_path(char *cmd_name, char **paths)
{
	char	*cmd_path;
	size_t	i;

	i = 0;
	while (paths[i] != NULL)
	{
		// TODO: Check if the command name is a route: "./" or "/"
		cmd_path = ft_strjoin(paths[i], cmd_name);
		if (cmd_path == NULL)
			return (NULL); // malloc error
		if (access(cmd_path, F_OK | X_OK) == 0)
		{
			free(cmd_name);
			return (cmd_path);
		}
		free(cmd_path);
		i++;
	}
	return (NULL);
}

char	***create_commands(int argc, char *argv[], char **paths)
{
	char	***commands;
	int		i;

	commands = (char ***)malloc(sizeof(char **) * (argc - 3));
	if (commands == NULL)
		return (NULL);
	i = 0;
	while (i + 2 < argc - 1)
	{
		commands[i] = ft_split(argv[i + 2], ' ');
		if (commands[i] == NULL)
			return (ft_free_str_trimatrix(commands));
		commands[i][0] = get_command_path(commands[i][0], paths);
		if (commands[i][0] == NULL)
			return (ft_free_str_trimatrix(commands));
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
	char	***commands;
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

	infile = open(argv[1], O_RDONLY);
	if (infile == -1)
	{
		ft_free_str_matrix(paths);
		perror("infile open fail");
		exit(EXIT_FAILURE);
	}

	outfile = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
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

	pipex(commands, envp, infile, outfile);

	ft_free_str_trimatrix(commands);
	ft_free_str_matrix(paths);
	close(infile);
	close(outfile);
	return (0);
}
