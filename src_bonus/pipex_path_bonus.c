/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_path_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migugar2 <migugar2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 21:25:04 by migugar2          #+#    #+#             */
/*   Updated: 2024/11/16 22:26:31 by migugar2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

int	get_envp_paths(char	***path, char **envp)
{
	char	*tmp;
	size_t	i;

	i = 0;
	while (envp != NULL && envp[i] != NULL)
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
		{
			*path = ft_split(envp[i] + 5, ':');
			if (*path == NULL)
				return (seterrno(ENOMEM), -1);
			i = 0;
			while (*path != NULL && (*path)[i] != NULL)
			{
				tmp = ft_strjoin((*path)[i], "/");
				if (tmp == NULL)
					return (ft_freestrarr(path), seterrno(12), -1);
				ft_freestr(&(*path)[i]);
				(*path)[i++] = tmp;
			}
			return (0);
		}
		i++;
	}
	return (-1);
}

// if fails and errno is not set, is command not found
int	get_cmd_path(char **cmd_path, const char *cmd_name, char **envp)
{
	size_t	i;
	char	**paths;

	if (ft_strchr(cmd_name, '/') != NULL)
	{
		*cmd_path = ft_strdup(cmd_name);
		if (*cmd_path == NULL)
			return (seterrno(ENOMEM), -1);
		return (0);
	}
	paths = NULL;
	if (get_envp_paths(&paths, envp) == -1)
		return (-1);
	i = 0;
	while (paths != NULL && paths[i] != NULL)
	{
		*cmd_path = ft_strjoin(paths[i], cmd_name);
		if (*cmd_path == NULL)
			return (ft_freestrarr(&paths), seterrno(ENOMEM), -1);
		if (access(*cmd_path, F_OK) == 0)
			return (ft_freestrarr(&paths), seterrno(0), 0);
		ft_freestr(cmd_path);
		i++;
	}
	return (ft_freestrarr(&paths), -1);
}
