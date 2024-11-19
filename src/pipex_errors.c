/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_errors.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migugar2 <migugar2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 19:00:56 by migugar2          #+#    #+#             */
/*   Updated: 2024/11/19 20:32:40 by migugar2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

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

int	error_handler_default(const char *arg)
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
		return (error_handler_default(arg));
	else if (error_code == 1)
	{
		ft_printf_error("pipex: usage: %s file1 cmd1 cmd2 file2\n", arg);
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
	return (error_handler_default(arg));
}

void	exit_execute_error(int errnum, char *command_name)
{
	int	status;

	seterrno(errnum);
	if (errnum == ENOENT)
		status = error_handler(2, command_name);
	else if (errnum == EACCES)
		status = error_handler(3, command_name);
	else
		status = error_handler(0, command_name);
	exit(status);
}
