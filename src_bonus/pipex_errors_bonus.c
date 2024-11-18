/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_errors_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migugar2 <migugar2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 21:18:55 by migugar2          #+#    #+#             */
/*   Updated: 2024/11/18 11:19:21 by migugar2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

static int	error_handler_2(int error_code, const char *arg)
{
	int	errnum;

	if (error_code == 4)
	{
		ft_printf_error(
			"pipex: usage: %s here_doc limiter cmd1 cmd2 ...cmdn file2\n", arg);
		return (EXIT_FAILURE);
	}
	errnum = errno;
	ft_printf_error("pipex: ");
	ft_perror(errnum, arg);
	return (EXIT_FAILURE);
}

int	error_handler(int error_code, const char *arg)
{
	if (error_code == 0)
		return (error_handler_2(error_code, arg));
	else if (error_code == 1)
	{
		ft_printf_error("pipex: usage: %s file1 cmd1 cmd2 ...cmdn file2\n",
			arg);
		return (EXIT_FAILURE);
	}
	else if (error_code == 2)
	{
		if (arg == NULL)
			ft_printf_error("pipex: command not found\n", arg);
		else
			ft_printf_error("pipex: command not found: %s\n", arg);
		return (127);
	}
	else if (error_code == 3)
	{
		if (arg == NULL)
			ft_printf_error("pipex: Permission denied\n", arg);
		else
			ft_printf_error("pipex: Permission denied: %s\n", arg);
		return (126);
	}
	return (error_handler_2(error_code, arg));
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
