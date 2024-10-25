/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_str_matrix_r.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migugar2 <migugar2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 22:12:10 by migugar2          #+#    #+#             */
/*   Updated: 2024/10/25 22:12:46 by migugar2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include <stdlib.h>

char	**ft_free_str_matrix_r(char **matrix, size_t i)
{
	if (matrix != NULL)
	{
		while (i > 0)
		{
			free(matrix[i - 1]);
			i--;
		}
		free(matrix);
	}
	return (NULL);
}
