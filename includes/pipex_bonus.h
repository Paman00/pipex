/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migugar2 <migugar2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 09:41:39 by migugar2          #+#    #+#             */
/*   Updated: 2024/11/01 13:13:22 by migugar2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_BONUS_H
# define PIPEX_BONUS_H

# include "../libft/libft.h"

# include <sys/types.h>

# include <errno.h>

# include <fcntl.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <sys/wait.h>

// TODO

char	*create_file_here_doc(char *limiter);
int		here_doc(int argc, char *argv[], char **envp, char **paths);

int		**create_pipes(int n_pipes);
pid_t	execute_first(char *argv[], char **envp, char **paths, int pipe_fd[2]);
pid_t	execute_middle(char *cmd_name, char **envp, char **paths, int **pipes);
pid_t	execute_last(char *argv[], char **envp, char **paths, int pipe_fd[2]);

void	exit_pipex(int error, const char *message, int pipe_fd[2]);
int		pipex(int argc, char *argv[], char **envp, char **paths);

#endif
