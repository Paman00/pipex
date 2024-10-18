/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migugar2 <migugar2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 17:01:00 by migugar2          #+#    #+#             */
/*   Updated: 2024/09/19 17:38:59 by migugar2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	i;
	size_t	l_dst;
	size_t	l_src;

	i = 0;
	l_dst = 0;
	l_src = 0;
	while (dst[l_dst] && l_dst < size)
		l_dst++;
	while (src[l_src])
		l_src++;
	if (size <= l_dst)
		return (size + l_src);
	while (src[i] && (l_dst + i + 1) < size)
	{
		dst[l_dst + i] = src[i];
		i++;
	}
	dst[l_dst + i] = 0;
	return (l_src + l_dst);
}
