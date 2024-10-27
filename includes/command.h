/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migugar2 <migugar2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 19:42:15 by migugar2          #+#    #+#             */
/*   Updated: 2024/10/27 19:51:07 by migugar2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_H
# define COMMAND_H

# include "../libft/libft.h"

# include <sys/types.h>

# include <errno.h>

# include <fcntl.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <sys/wait.h>

void	exit_error(int error_code, const char *message, char **paths);

char	**get_paths(char **envp);
char	*get_cmd_path(char *cmd_name, char **paths);
char	**create_cmd(char *cmd_name, char **paths);
void	execute_cmd(char **command, char **envp, int in_fd, int out_fd);

#endif
