/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_errors_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migugar2 <migugar2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 21:18:55 by migugar2          #+#    #+#             */
/*   Updated: 2024/11/19 20:26:34 by migugar2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

static void	p_error_command(char *msg, const char *command)
{
	int		isonlyspace;
	size_t	i;

	if (!command)
		ft_printf_error("pipex: %s\n", msg);
	isonlyspace = 1;
	i = 0;
	while (isonlyspace && command[i] != '\0')
	{
		if (command[i] != ' ')
			isonlyspace = 0;
		i++;
	}
	if (isonlyspace)
		ft_printf_error("pipex: %s: %s\n", msg, command);
	else
	{
		i = 0;
		while (command[i] && command[i] == ' ')
			i++;
		ft_printf_error("pipex: %s: ", msg);
		while (command[i] && command[i] != ' ')
			ft_putchar_fd(STDERR_FILENO, command[i++]);
		ft_putchar_fd(STDERR_FILENO, '\n');
	}
}

static int	error_handler_2(const char *arg)
{
	int	errnum;

	errnum = errno;
	ft_printf_error("pipex: ");
	ft_perror(errnum, arg);
	return (EXIT_FAILURE);
}

int	error_handler(int error_code, const char *arg)
{
	if (error_code == 0)
		return (error_handler_2(arg));
	else if (error_code == 1)
	{
		ft_printf_error("pipex: usage: %s file1 cmd1 cmd2 ...cmdn file2\n",
			arg);
		return (EXIT_FAILURE);
	}
	else if (error_code == 2)
	{
		p_error_command("command not found", arg);
		return (127);
	}
	else if (error_code == 3)
	{
		p_error_command("permission denied", arg);
		return (126);
	}
	else if (error_code == 4)
	{
		ft_printf_error(
			"pipex: usage: %s here_doc limiter cmd1 cmd2 ...cmdn file2\n", arg);
		return (EXIT_FAILURE);
	}
	return (error_handler_2(arg));
}

int	error_handler_free(int error_code, char *arg)
{
	int	status;

	status = error_handler(error_code, arg);
	ft_freestr(&arg);
	return (status);
}

void	exit_execute_error(int errnum, char *command_name)
{
	seterrno(errnum);
	if (errnum == ENOENT)
		exit(error_handler(2, command_name));
	if (errnum == EACCES)
		exit(error_handler(3, command_name));
	exit(error_handler(0, NULL));
}
