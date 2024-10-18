/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migugar2 <migugar2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 14:22:36 by migugar2          #+#    #+#             */
/*   Updated: 2024/10/18 20:45:57 by migugar2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stddef.h>

static int	set_contains(char const *set, char c)
{
	size_t	i;

	i = 0;
	while (set[i])
	{
		if (c == set[i])
			return (1);
		i++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*r;
	size_t	l_trim;
	size_t	r_trim;
	size_t	s_len;

	if (s1 == NULL || set == NULL)
		return (NULL);
	l_trim = 0;
	while (s1[l_trim] && set_contains(set, s1[l_trim]))
		l_trim++;
	s_len = l_trim;
	while (s1[s_len])
		s_len++;
	r_trim = s_len - 1;
	while (r_trim > l_trim && set_contains(set, s1[r_trim]))
		r_trim--;
	r = ft_substr(s1, l_trim, r_trim - l_trim + 1);
	return (r);
}
