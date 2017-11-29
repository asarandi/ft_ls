/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asarandi <asarandi@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/20 15:37:35 by asarandi          #+#    #+#             */
/*   Updated: 2017/09/24 14:00:23 by asarandi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

#define INT_MAX	0x7fffffff
#define INT_MIN	0x80000000

static int	ft_atoi_limit(int base, char c, int neg)
{
	unsigned long	x;
	unsigned long	y;
	unsigned long	z;

	x = base;
	z = c - '0';
	y = INT_MAX;
	if ((x >= y / 10) && (neg == 0))
		if (z > y % 10)
			return (-1);
	y = INT_MIN;
	if ((x >= y / 10) && (neg != 0))
		if (z > y % 10)
			return (0);
	return (1);
}

int			ft_atoi(const char *s)
{
	int	i;
	int	n;
	int	r;

	i = 0;
	n = 0;
	while (ft_isspace(s[i]))
		i++;
	if (s[i] == '+')
		i++;
	else if (s[i] == '-')
		n = ++i;
	r = 0;
	while (ft_isdigit(s[i]))
	{
		if (ft_atoi_limit(r, s[i], n) != 1)
			return (ft_atoi_limit(r, s[i], n));
		r = (r * 10) + s[i++] - '0';
	}
	if (n)
		return (-r);
	else
		return (r);
}
