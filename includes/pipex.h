/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migugar2 <migugar2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 20:52:37 by migugar2          #+#    #+#             */
/*   Updated: 2024/10/27 02:52:12 by migugar2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "../libft/libft.h"

# include <sys/types.h>

# include <errno.h>

# include <fcntl.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <sys/wait.h>

char	**get_paths(char **envp);
char	*get_cmd_path(char *cmd_name, char **paths);
char	**create_cmd(char *cmd, char **paths);
void	execute_cmd(char **command, char **envp, int in_fd, int out_fd);

void	exit_pipex(int error_code, const char *message, int pipe_fd[2]);
pid_t	execute_first(char *argv[], char **envp, char **paths, int pipe_fd[2]);
pid_t	execute_second(char *argv[], char **envp, char **paths, int pipe_fd[2]);
int		pipex(char *argv[], char **envp, char **paths);

#endif
