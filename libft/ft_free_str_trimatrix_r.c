/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_str_trimatrix_r.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migugar2 <migugar2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 21:38:36 by migugar2          #+#    #+#             */
/*   Updated: 2024/10/25 22:13:02 by migugar2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include <stdlib.h>
#include "libft.h"

char	***ft_free_str_trimatrix_r(char ***matrix, size_t i)
{
	if (matrix != NULL)
	{
		while (i > 0)
		{
			ft_free_str_matrix(matrix[i - 1]);
			i--;
		}
		free(matrix);
	}
	return (NULL);
}
