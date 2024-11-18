/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migugar2 <migugar2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 09:41:39 by migugar2          #+#    #+#             */
/*   Updated: 2024/11/18 11:15:30 by migugar2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_BONUS_H
# define PIPEX_BONUS_H

# include "libft.h"

# include <sys/types.h>

# include <errno.h>

# include <fcntl.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <sys/wait.h>

int		error_handler(int error_code, const char *arg);
int		error_handler_free(int error_code, char *arg);
void	exit_execute_error(int errnum, char *command_name);

int		execute_cmd(char *command_name, char **envp, int in_fd, int out_fd);

int		execute_first(char *cmd_name, char **envp, char *file_name);
int		execute_middle(char *cmd_name, char **envp, int pipe_in);
pid_t	execute_last(char *argv_last[], char **envp, int pipe_in, int oflag);

void	wait_processes(int waitc);

# define PS2 "heredoc\033[31m|> \033[0m"

char	*create_tmpfile_name(char *basename);
char	*read_in_tmpfile(int in_fd, char *limiter, size_t limiter_len);
int		here_doc(int argc, char *argv[], char **envp);

int		get_envp_paths(char	***path, char **envp);
int		get_cmd_path(char **cmd_path, const char *cmd_name, char **envp);

int		pipex(int argc, char *argv[], char **envp, int oflag);

#endif
