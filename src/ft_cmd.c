/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cmd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migugar2 <migugar2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 02:39:08 by migugar2          #+#    #+#             */
/*   Updated: 2024/10/27 02:52:18 by migugar2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

char	**get_paths(char **envp)
{
	char	**paths;
	char	*tmp;
	size_t	i;

	i = 0;
	while (envp != NULL && envp[i] != NULL)
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
		{
			paths = ft_split(envp[i] + 5, ':');
			i = 0;
			while (paths != NULL && paths[i] != NULL)
			{
				tmp = ft_strjoin(paths[i], "/");
				if (tmp == NULL)
					return (ft_free_str_matrix_r(paths, i));
				free(paths[i]);
				paths[i] = tmp;
				i++;
			}
			return (paths);
		}
		i++;
	}
	return (NULL);
}

char	*get_cmd_path(char *cmd_name, char **paths)
{
	char	*cmd_path;
	size_t	i;

	i = 0;
	if (ft_strchr(cmd_name, '/') != NULL)
		return (ft_strdup(cmd_name));
	while (paths[i] != NULL)
	{
		cmd_path = ft_strjoin(paths[i], cmd_name);
		if (cmd_path == NULL)
			return (NULL);
		if (access(cmd_path, X_OK) == 0)
			return (cmd_path);
		free(cmd_path);
		i++;
	}
	return (NULL);
}

char	**create_cmd(char *cmd, char **paths)
{
	char	**command;
	char	*cmd_path;

	command = ft_split(cmd, ' ');
	if (command == NULL)
		return (NULL);
	cmd_path = get_cmd_path(command[0], paths);
	if (cmd_path == NULL)
		return (ft_free_str_matrix(command));
	free(command[0]);
	command[0] = cmd_path;
	return (command);
}

void	execute_cmd(char **command, char **envp, int in_fd, int out_fd)
{
	if (in_fd != 0)
	{
		dup2(in_fd, 0);
		close(in_fd);
	}
	if (out_fd != 1)
	{
		dup2(out_fd, 1);
		close(out_fd);
	}
	execve(command[0], command, envp);
	if (errno == ENOENT)
	{
		perror("command not found");
		exit(127);
	}
	if (errno == EACCES)
	{
		perror("permission denied");
		exit(126);
	}
	perror("execve fail");
	exit(EXIT_FAILURE);
}
