/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migugar2 <migugar2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 09:41:39 by migugar2          #+#    #+#             */
/*   Updated: 2024/10/27 19:45:01 by migugar2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_BONUS_H
# define PIPEX_BONUS_H

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

char	*create_file_here_doc(int argc, char *argv[]);
int		here_doc(int argc, char *argv[], char **envp, char **paths);

int		**create_pipes(size_t n_pipes);
pid_t	execute_first(char *argv[], char **envp, char **paths, int pipe_fd[2]);
pid_t	execute_middle(char *cmd_name, char **envp, char **paths, int **pipes);
pid_t	execute_last(char *argv[], char **envp, char **paths, int pipe_fd[2]);

void	exit_pipex(int error, int n_errno, const char *message, int pipe_fd[2]);
int		pipex(int argc, char *argv[], char **envp, char **paths);

#endif
