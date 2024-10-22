/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migugar2 <migugar2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 22:45:04 by migugar2          #+#    #+#             */
/*   Updated: 2024/10/22 23:02:43 by migugar2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

// < archivo1 comando1 | comando2 > archivo2

// pipe() devuelve un arr[] de tipo int que tiene dos extremos, cada extremo
// es un fd, el [0] es para leer y el [1] es para escribir, lo que se escribe
// en el [1] se lee en el [0], es decir, se escribe en el pipe y se lee del pipe
// gracias a que el pipe es un buffer en memoria.

void	execute_command(char *cmd[], int input_fd, int output_fd)
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
		execve(cmd[0], cmd, NULL);
		perror("execve failed");
		exit(EXIT_FAILURE);
	}
}

void	pipex(/*int argc,*/ char *argv[]) //
{
	int		infile;
	int		outfile;
	int		pipe1[2];
	// int		pipe2[2];
	size_t	i;
	char	**cmd1;
	char	**cmd2;

	infile = open(argv[1], O_RDONLY);
	if (infile == -1)
	{
		perror("infile open fail");
		exit(EXIT_FAILURE);
	}

	outfile = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (outfile == -1)
	{
		close(infile);
		perror("outfile open fail");
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
	while (i < 2)
	{
		wait(NULL); // Migrate to waitpid, so execute_command must return the exact pid_t
		i++;
	}
}

int	main(int argc, char *argv[])
{
	if (argc != 5)
	{
		perror("argc fail");
		exit(EXIT_FAILURE);
	}

	pipex(/*argc,*/ argv);

	return (0);
}
