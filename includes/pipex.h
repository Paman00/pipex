/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migugar2 <migugar2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 20:52:37 by migugar2          #+#    #+#             */
/*   Updated: 2024/10/27 19:49:41 by migugar2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "../libft/libft.h"
# include "../includes/command.h"

# include <sys/types.h>

# include <errno.h>

# include <fcntl.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <sys/wait.h>

pid_t	execute_first(char *argv[], char **envp, char **paths, int pipe_fd[2]);
pid_t	execute_second(char *argv[], char **envp, char **paths, int pipe_fd[2]);

void	exit_pipex(int error_code, const char *message, int pipe_fd[2]);
int		pipex_simple(char *argv[], char **envp, char **paths);

#endif
