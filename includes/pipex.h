/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migugar2 <migugar2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 20:52:37 by migugar2          #+#    #+#             */
/*   Updated: 2024/11/16 21:21:40 by migugar2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "libft.h"

# include <sys/types.h>

# include <errno.h>

# include <fcntl.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <sys/wait.h>

int		error_handler_default(const char *arg);
int		error_handler(int error_code, const char *arg);
void	exit_execute_error(int errnum, char *command_name);

int		execute_cmd(char *command_name, char **envp, int in_fd, int out_fd);
pid_t	execute_first(char *argv[], char **envp, int pipe_fd[2]);
pid_t	execute_second(char *argv[], char **envp, int pipe_fd[2]);

int		get_envp_paths(char	***path, char **envp);
int		get_cmd_path(char **cmd_path, const char *cmd_name, char **envp);

int		pipex(char *argv[], char **envp);

/*
pid_t	execute_first(char *argv[], char **envp, char **paths, int pipe_fd[2]);
pid_t	execute_second(char *argv[], char **envp, char **paths, int pipe_fd[2]);

void	exit_pipex(int error_code, const char *message, int pipe_fd[2]);
int		pipex_simple(char *argv[], char **envp, char **paths);
*/

#endif
