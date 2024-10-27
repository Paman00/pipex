/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migugar2 <migugar2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 13:53:15 by migugar2          #+#    #+#             */
/*   Updated: 2024/10/27 10:48:48 by migugar2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stddef.h>
#include <stdlib.h>

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*r;
	size_t	s1_len;
	size_t	s2_len;
	size_t	i;

	if (s1 == NULL && s2 == NULL)
		return (NULL);
	s1_len = ft_strlen(s1);
	s2_len = ft_strlen(s2);
	r = (char *)malloc(sizeof(char) * (s1_len + s2_len + 1));
	if (r == NULL)
		return (NULL);
	i = 0;
	while (i < s1_len || i < s2_len)
	{
		if (i < s1_len)
			r[i] = s1[i];
		if (i < s2_len)
			r[s1_len + i] = s2[i];
		i++;
	}
	r[s1_len + s2_len] = '\0';
	return (r);
}
