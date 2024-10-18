/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migugar2 <migugar2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 18:46:47 by migugar2          #+#    #+#             */
/*   Updated: 2024/10/18 20:42:48 by migugar2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stddef.h>
#include <stdlib.h>

static size_t	get_n_len(int n)
{
	size_t	i;

	i = 0;
	while (n != 0)
	{
		n /= 10;
		i++;
	}
	return (i);
}

static char	*get_r(int n, int sign, size_t i)
{
	char	*r;

	r = (char *)malloc(sizeof(char) * (i + 1));
	if (r == NULL)
		return (NULL);
	r[i] = '\0';
	i--;
	while (n != 0)
	{
		r[i] = (n % 10) + '0';
		n /= 10;
		i--;
	}
	if (sign == -1)
		r[i] = '-';
	return (r);
}

char	*ft_itoa(int n)
{
	int		sign;
	size_t	i;

	if (n == -2147483648)
		return (ft_strdup("-2147483648"));
	if (n == 0)
		return (ft_strdup("0"));
	i = 0;
	sign = 1;
	if (n < 0)
	{
		sign = -1;
		n = -n;
		i++;
	}
	i += get_n_len(n);
	return (get_r(n, sign, i));
}
