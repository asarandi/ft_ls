/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asarandi <asarandi@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/25 20:05:13 by asarandi          #+#    #+#             */
/*   Updated: 2017/12/02 23:17:06 by asarandi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_LS_H
# define FT_LS_H

# include <dirent.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <pwd.h>
# include <grp.h>
# include <uuid/uuid.h>
# include <sys/xattr.h>
# include <time.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <termios.h>
# include <sys/ioctl.h>
# include <errno.h>
# include "libft/libft.h"
# include "libft/ft_printf/libftprintf.h"
# include <sys/acl.h>

typedef struct		s_file
{
	char			*name;
	struct stat		st;
	struct s_file	*next;
}					t_file;

typedef struct		s_opt
{
	int	long_list;
	int	recursive;
	int	reverse;
	int	show_dot;
	int	show_hidden;
	int	time;
	int	sort;
	int	(*sort_algo)(t_file *f1, t_file *f2);
	int	symbol;
	int	slash;
	int	colors;
	int	fulltime;
	int	hide_owner;
	int	hide_group;
	int	show_acl;
	int	show_extattr;
	int	plain;
	int	last_opt;
}					t_op;

t_op				g_opt;
char				*g_ls_name;

void				sort_list(t_file **list);
void				print_entry_type(unsigned long st_mode);
void				print_entry_symbol(unsigned long st_mode);
void				print_permissions(unsigned long st_mode);
void				print_file_mode(unsigned long st_mode);
int					sort_by_atime_asc(t_file *file1, t_file *file2);
int					sort_by_atime_desc(t_file *file1, t_file *file2);
int					sort_by_btime_asc(t_file *file1, t_file *file2);
int					sort_by_btime_desc(t_file *file1, t_file *file2);
int					sort_by_ctime_asc(t_file *file1, t_file *file2);
int					sort_by_ctime_desc(t_file *file, t_file *file2);
int					sort_by_ctime_desc(t_file *file1, t_file *file2);
int					sort_by_mtime_asc(t_file *file1, t_file *file2);
int					sort_by_mtime_desc(t_file *file1, t_file *file2);
int					sort_by_name_asc(t_file *file1, t_file *file2);
int					sort_by_name_desc(t_file *file1, t_file *file2);
int					sort_by_size_asc(t_file *file1, t_file *file2);
int					sort_by_size_desc(t_file *file1, t_file *file2);

#endif
