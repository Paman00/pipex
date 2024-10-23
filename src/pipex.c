/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migugar2 <migugar2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 22:45:04 by migugar2          #+#    #+#             */
/*   Updated: 2024/10/23 22:46:50 by migugar2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

// < archivo1 comando1 | comando2 > archivo2

// pipe() devuelve un arr[] de tipo int que tiene dos extremos, cada extremo
// es un fd, el [0] es para leer y el [1] es para escribir, lo que se escribe
// en el [1] se lee en el [0], es decir, se escribe en el pipe y se lee del pipe
// gracias a que el pipe es un buffer en memoria.

void	execute_command(char *cmd[], char **envp, int input_fd, int output_fd)
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
		if (input_fd != 0)
		{
			dup2(input_fd, 0);
			close(input_fd);
		}
		if (output_fd != 1)
		{
			dup2(output_fd, 1);
			close(output_fd);
		}
		execve(cmd[0], cmd, envp);
		// TODO: The first string in cmd must be the command and not the path, for the correct execution of execve
		perror("execve failed");
		exit(EXIT_FAILURE);
	}
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
	free(commands[0]);
	execute_command(commands[1], envp, pipe1[0], outfile);
	close(pipe1[0]);
	free(commands[1]);
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

char	*get_cmd(char **cmd_name, char **paths)
{
	char	*cmd_path;
	size_t	i;

	i = 0;
	while (paths[i] != NULL)
	{
		cmd_path = ft_strjoin(paths[i], *cmd_name);
		printf("Checking cmd %s like path %s\n", *cmd_name, cmd_path);
		if (cmd_path == NULL)
			return (NULL); // malloc error
		if (access(cmd_path, F_OK | X_OK) == 0)
		{
			free(*cmd_name);
			*cmd_name = cmd_path;
			return (*cmd_name);
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
	if (!commands)
		return (NULL);
	i = 0;
	while (i + 2 < argc - 1)
	{
		commands[i] = ft_split(argv[i + 2], ' ');
		if (commands[i] == NULL)
		{
			// TODO: free the double matrix
			return (NULL);
		}
		printf("For %d of %d - command %s\n", i + 2, argc, commands[i][0]);
		if (get_cmd(&commands[i][0], paths) == NULL)
		{
			// TODO: free the double matrix
			return (NULL);
		}
		i++;
	}
	commands[i - 2] = NULL;
	return (commands);
}

char	**get_paths(char **envp)
{
	size_t	i;
	char	**paths;
	char	*tmp;

	i = 0;
	while (envp && envp[i] != NULL)
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
		{
			paths = ft_split(envp[i] + 5, ':');
			if (paths == NULL)
				return (NULL); // TODO
			i = 0;
			while (paths[i] != NULL)
			{
				tmp = ft_strjoin(paths[i], "/");
				if (tmp == NULL)
					return (NULL); // TODO
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
		free(paths);
		perror("infile open fail");
		exit(EXIT_FAILURE);
	}

	outfile = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (outfile == -1)
	{
		free(paths);
		close(infile);
		perror("outfile open fail");
		exit(EXIT_FAILURE);
	}

	commands = create_commands(argc, argv, paths);
	if (commands == NULL)
	{
		free(paths);
		close(infile);
		close(outfile);
		perror("commands creation fail");
		exit(EXIT_FAILURE);
	}

	pipex(commands, envp, infile, outfile);

	free(paths);
	close(infile);
	close(outfile);
	return (0);
}
