/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migugar2 <migugar2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 09:41:39 by migugar2          #+#    #+#             */
/*   Updated: 2024/11/16 22:29:48 by migugar2         ###   ########.fr       */
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

int		error_handler_default(const char *arg);
int		error_handler(int error_code, const char *arg);
int		error_handler_free(int error_code, char *arg);
void	exit_execute_error(int errnum, char *command_name);

char	*create_tmpfile_name(char *basename);
char	*read_in_tmpfile(int in_fd, char *limiter);
int		here_doc(int argc, char *argv[], char **envp);

int		get_envp_paths(char	***path, char **envp);
int		get_cmd_path(char **cmd_path, const char *cmd_name, char **envp);

#endif
