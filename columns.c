/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   columns.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asarandi <asarandi@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/04 05:22:23 by asarandi          #+#    #+#             */
/*   Updated: 2017/12/04 17:14:39 by asarandi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void	get_max_filename_len(t_file *list)
{
	size_t	len;

	len = ft_strlen(list->name);
	len += has_symbol(list->st.st_mode);
	if (len > g_width.name)
		g_width.name = len;
}

void	print_tabs(intmax_t name_len)
{
	int	col_width;
	int nam_width;

	col_width = (g_width.name / TAB_SIZE) + 1;
	nam_width = (name_len / TAB_SIZE);
	while (nam_width < col_width)
	{
		write(1, "\t", 1);
		nam_width += 1;
	}
}

void	column_output(int num_col, t_file *list)
{
	uintmax_t	i;
	intmax_t	len;

	i = 0;
	while (list)
	{
		ft_printf(1, "%s", list->name);
		print_entry_symbol(list->st.st_mode);
		if (list->next == NULL)
			break ;
		if ((i + 1) % num_col == 0)
			ft_printf(1, "\n");
		else
		{
			len = ft_strlen(list->name);
			print_tabs(len + has_symbol(list->st.st_mode));
		}
		list = list->next;
		i++;
	}
	write(1, "\n", 1);
}

void	print_columns(t_file *list)
{
	print_basic(list);
}

void	print_across(t_file *list)
{
	struct winsize	ws;
	int				num_col;
	int				tabs_longest_name;

	if (isatty(1) == 10)
	{
		print_basic(list);
		return ;
	}
	if ((num_col = ioctl(0, TIOCGWINSZ, &ws)) == -1)
	{
		print_basic(list);
		return ;
	}
//	ft_printf(1, "TIOCGWINSZ (returns %d) ws_row: %d, ws_col: %d, ws_xpixel: %d, ws_ypixel: %d\n", num_col, ws.ws_row, ws.ws_col, ws.ws_xpixel, ws.ws_ypixel);
	get_directory_widths(list);
	tabs_longest_name = (g_width.name / TAB_SIZE) + 1;
	num_col = (ws.ws_col / TAB_SIZE) / tabs_longest_name;
//	ft_printf(1, "num col is %d\n", num_col);
	column_output(num_col, list);
}
