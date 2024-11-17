/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_errors.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migugar2 <migugar2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 19:00:56 by migugar2          #+#    #+#             */
/*   Updated: 2024/11/17 22:25:47 by migugar2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

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
	if (error_code == 1)
	{
		ft_printf_error("pipex: usage: %s file1 cmd1 cmd2 file2\n", arg);
		return (EXIT_FAILURE);
	}
	if (error_code == 2)
	{
		if (arg == NULL)
			ft_printf_error("pipex: command not found\n", arg);
		else
			ft_printf_error("pipex: command not found: %s\n", arg);
		return (127);
	}
	if (error_code == 3)
	{
		if (arg == NULL)
			ft_printf_error("pipex: Permission denied\n", arg);
		else
			ft_printf_error("pipex: Permission denied: %s\n", arg);
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
