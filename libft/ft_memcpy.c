/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migugar2 <migugar2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 20:41:06 by migugar2          #+#    #+#             */
/*   Updated: 2024/09/21 11:38:44 by migugar2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	unsigned char	*p;
	size_t			i;

	if (dest == NULL && src == NULL)
		return (NULL);
	p = (unsigned char *)dest;
	i = 0;
	while (i < n)
	{
		p[i] = ((unsigned char *)src)[i];
		i++;
	}
	return (dest);
}
