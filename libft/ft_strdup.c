/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migugar2 <migugar2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 18:58:41 by migugar2          #+#    #+#             */
/*   Updated: 2024/10/18 20:54:51 by migugar2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include <stdlib.h>
#include "libft.h"

char	*ft_strdup(const char *s)
{
	char	*s_dup;
	size_t	l;
	size_t	i;

	l = ft_strlen(s);
	s_dup = malloc(sizeof(char) * (l + 1));
	if (s_dup == NULL)
		return (NULL);
	i = 0;
	while (i < l)
	{
		s_dup[i] = (char)s[i];
		i++;
	}
	s_dup[i] = '\0';
	return (s_dup);
}
