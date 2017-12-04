/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asarandi <asarandi@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/03 18:32:06 by asarandi          #+#    #+#             */
/*   Updated: 2017/12/03 18:55:27 by asarandi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void	print_basic(t_file *list)
{
	while (list)
	{
		ft_printf(1, "%s", list->name);
		print_entry_symbol(list->st.st_mode);
		ft_printf(1, "\n");
		list = list->next;
	}
}

void	print_list(char *path, t_file *list)
{
	struct passwd	*pwd;
	struct group	*grp;
	char			*symlink;
	struct timespec	ts;

	if ((list) && (g_opt.long_list == 0))
	{
		print_basic(list);
		return ;
	}

	get_directory_widths(list);
	if (*path)
		ft_printf(1, "total %llu\n", g_width.blocks);
	while (list != NULL)
	{
		print_file_mode(list->st.st_mode);
		print_extended_attributes(path, list);
		ft_printf(1, " %*u ", g_width.links, list->st.st_nlink);
		pwd = getpwuid(list->st.st_uid);
		if ((pwd != NULL) && (g_opt.hide_owner == 0))
			ft_printf(1, "%-*s  ", g_width.owner, pwd->pw_name);
		grp = getgrgid(list->st.st_gid);
		if ((grp != NULL) && (g_opt.hide_group == 0))
			ft_printf(1, "%-*s  ", g_width.group, grp->gr_name);
		if ((g_opt.hide_owner == 1) && (g_opt.hide_group == 1))
			ft_printf(1, "  ");
		if (g_width.has_cb)
		{
			if ((S_ISCHR(list->st.st_mode)) || (S_ISBLK(list->st.st_mode)))
			{
				ft_printf(1, "%3d, ", major(list->st.st_rdev));
				if ((minor(list->st.st_rdev) > 255) || (minor(list->st.st_rdev) < 0))
					ft_printf(1, "0x%08x ", minor(list->st.st_rdev));
				else
					ft_printf(1, "%3d ", minor(list->st.st_rdev));
			}
			else
				ft_printf(1, "%*s%*llu ", 8 - g_width.size, "", g_width.size, list->st.st_size);
		}
		else
			ft_printf(1, "%*llu ", g_width.size, list->st.st_size);

		if (g_opt.time == 1)
			ts = list->st.st_atimespec;
		else if (g_opt.time == 3)
			ts = list->st.st_ctimespec;
		else if (g_opt.time == 2)
			ts = list->st.st_birthtimespec;
		else
			ts = list->st.st_mtimespec;

		if (g_opt.fulltime == 1)
			ft_printf(1, "%.20s ", ctime(&ts.tv_sec)+4);

		else
			ft_printf(1, "%.12s ", make_time_string(ts));
		ft_printf(1, "%s", list->name);
		print_entry_symbol(list->st.st_mode);
		if (S_ISLNK(list->st.st_mode))
		{
			symlink = get_symlink_address(path, list);
			if (symlink != NULL)
			{
				ft_printf(1, " -> %s", symlink);
				free(symlink);
			}
		}
		ft_printf(1, "\n");
		list = list->next;
	}
}
