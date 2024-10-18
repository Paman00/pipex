/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migugar2 <migugar2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 14:59:23 by migugar2          #+#    #+#             */
/*   Updated: 2024/09/21 10:24:47 by migugar2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

char	*ft_strrchr(const char *s, int c)
{
	unsigned char	*p;
	char			*r;

	r = NULL;
	p = (unsigned char *)s;
	while (*p)
	{
		if (*p == (unsigned char)c)
			r = (char *)p;
		p++;
	}
	if ((unsigned char)c == '\0')
		return ((char *)p);
	return (r);
}
