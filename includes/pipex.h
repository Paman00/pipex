/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migugar2 <migugar2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 20:52:37 by migugar2          #+#    #+#             */
/*   Updated: 2024/10/23 22:49:56 by migugar2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "../libft/libft.h"

# include <sys/types.h> // id_t

# include <fcntl.h> // open
# include <unistd.h> // close, read, write, access, dup, dup2, execve, fork, pipe, unlink
# include <stdlib.h> // malloc, free, exit
# include <stdio.h> // perror
# include <string.h> // strerror
# include <sys/wait.h> // wait, waitpid

typedef struct s_command
{
	char		**argv;
	char		*cmd;
	const char	*envp;
}	t_command;

#endif
