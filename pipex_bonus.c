/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migugar2 <migugar2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 06:08:29 by migugar2          #+#    #+#             */
/*   Updated: 2024/11/16 22:34:37 by migugar2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

int	main(int argc, char *argv[], char **envp)
{
	int	last_status;

	if (argc < 5)
		exit(error_handler(1, argv[0]));
	if (ft_strncmp(argv[1], "here_doc", 9) == 0)
	{
		if (argc < 6)
			exit(error_handler(1, argv[0]));
		last_status = here_doc(argc, argv, envp);
	}
}
