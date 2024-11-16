/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_errors_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migugar2 <migugar2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 21:18:55 by migugar2          #+#    #+#             */
/*   Updated: 2024/11/16 22:32:29 by migugar2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

int	error_handler_default(const char *arg)
{
	int	errnum;

	errnum = errno;
	ft_printf_error("pipex: ");
	ft_perror(errnum, arg);
	return (EXIT_FAILURE);
}

// 1-argc fail, 2-cmd not found, 3-permission denied, 0-errno
int	error_handler(int error_code, const char *arg)
{
	if (error_code == 0)
		return (error_handler_default(arg));
	if (error_code == 1)
	{
		ft_printf_error("pipex: usage: %s file1 cmd1 cmd2 ...cmdn file2\n",
			arg);
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

int	error_handler_free(int error_code, char *arg)
{
	int	errnum;
	int	status;

	errnum = errno;
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
