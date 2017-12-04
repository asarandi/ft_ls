/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_options.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asarandi <asarandi@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/03 18:29:57 by asarandi          #+#    #+#             */
/*   Updated: 2017/12/03 18:55:13 by asarandi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void	illegal_option(char c)
{
	ft_printf(2, "%s: illegal option -- %c\n", g_ls_name, c);
	ft_printf(2, "usage: ls [-AacdeFfGglopRrSTtUu@1] [file ...]\n");
	exit(0);
}

void	parse_options(int ac, char **av)
{
	int			i;
	int			k;

	g_opt.last_opt = 1;
	(void)clear_options();
	i = 1;
	while (i < ac)
	{
		while ((av[i]) && (av[i][0] == '-'))
		{
			if ((av[i][1] == '-') && (av[i][2] == 0))
			{
				g_opt.last_opt = ++i;
				return ;
			}
			k = 1;
			while (av[i][k])
			{
				if (av[i][k] == 'l')
					g_opt.long_list = 1;
				else if (av[i][k] == '1')
					g_opt.long_list = 0;
				else if (av[i][k] == 'R')
					g_opt.recursive = 1;
				else if (av[i][k] == 'r')
					g_opt.reverse = 1;
				else if (av[i][k] == 'a')
					g_opt.show_dot = 1;
				else if (av[i][k] == 'A')
					g_opt.show_hidden = 1;
				else if (av[i][k] == 't')
				{	
					if (g_opt.sort != 4)
						g_opt.sort = 1;
				}
				else if (av[i][k] == 'S')
					g_opt.sort = 4;
				else if (av[i][k] == 'u')
					g_opt.time = 1;
				else if (av[i][k] == 'U')
					g_opt.time = 2;
				else if (av[i][k] == 'c')
					g_opt.time = 3;
				else if (av[i][k] == 'f')
				{
					g_opt.sort = -1;
					g_opt.show_dot = 1;
				}
				else if (av[i][k] == 'T')
					g_opt.fulltime = 1;
				else if (av[i][k] == 'F')
				{
					g_opt.symbol = 1;
					g_opt.slash = 0;
				}
				else if (av[i][k] == 'p')
				{
					g_opt.slash = 1;
					g_opt.symbol = 0;
				}
				else if (av[i][k] == 'G')
					g_opt.colors = 1;
				else if (av[i][k] == 'g')
				{
					g_opt.hide_owner = 1;
					g_opt.long_list = 1;
				}
				else if (av[i][k] == 'o')
				{
					g_opt.hide_group = 1;
					g_opt.long_list = 1;
				}
				else if (av[i][k] == 'e')
					g_opt.show_acl = 1;
				else if (av[i][k] == '@')
					g_opt.show_extattr = 1;
				else if (av[i][k] == 'd')
					g_opt.plain = 1;
				else
					(void)illegal_option(av[i][k]);
				k++;
			}
			i++;
			g_opt.last_opt = i;
		}
		return ;
	}
	return ;
}
