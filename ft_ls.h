/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asarandi <asarandi@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/25 20:05:13 by asarandi          #+#    #+#             */
/*   Updated: 2017/12/03 19:02:05 by asarandi         ###   ########.fr       */
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
# include "ft_printf/libftprintf.h"
# include <sys/acl.h>

typedef unsigned long long	t_x;

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

typedef struct	s_width
{
	t_x	links;
	t_x	owner;
	t_x	group;
	t_x	size;
	t_x	blocks;
	t_x	has_cb;
}				t_wd;

t_op				g_opt;
t_wd				g_width;

char				*g_ls_name;

char	*get_symlink_address(char *path, t_file *list);
char	*make_time_string(struct timespec ts);
int		check_extended_attributes(char *path, t_file *list);
int		is_allowed(char *fn);
int		is_directory(char *path);
int		ok_to_recurse(char *path);
int		sort_by_atime_asc(t_file *file1, t_file *file2);
int		sort_by_atime_desc(t_file *file1, t_file *file2);
int		sort_by_btime_asc(t_file *file1, t_file *file2);
int		sort_by_btime_desc(t_file *file1, t_file *file2);
int		sort_by_ctime_asc(t_file *file1, t_file *file2);
int		sort_by_ctime_desc(t_file *file1, t_file *file2);
int		sort_by_mtime_asc(t_file *file1, t_file *file2);
int		sort_by_mtime_desc(t_file *file1, t_file *file2);
int		sort_by_name_asc(t_file *file1, t_file *file2);
int		sort_by_name_desc(t_file *file1, t_file *file2);
int		sort_by_size_asc(t_file *file1, t_file *file2);
int		sort_by_size_desc(t_file *file1, t_file *file2);
int		count_digits(unsigned long long	n);
int		display_files(int ac, char **av);
int		get_file_stats(char *path, t_file *file);
int		main(int ac, char **av);
t_file	*create_list(char *path);
t_file	*build_directory_list(int ac, char **av);
t_file	*build_file_list(int ac, char **av);
void	calculate_widths();
void	choose_sort();
void	clear_options();
void	clear_widths();
void	destroy_list(t_file *list);
void	directory_add_slash(char **path);
void	display_directories(int ac, char **av,  int flag);
void	get_directory_widths(t_file *list);
void	get_str_group_width(t_x *save, gid_t gid);
void	get_str_owner_width(t_x *save, uid_t uid);
void	illegal_option(char c);
void	list_directory(char *path);
void	list_split(t_file *head, t_file **left, t_file **right);
void	parse_options(int ac, char **av);
void	print_basic(t_file *list);
void	print_entry_symbol(unsigned long st_mode);
void	print_entry_type(unsigned long st_mode);
void	print_extended_attributes(char *path, t_file *file);
void	print_file_mode(unsigned long st_mode);
void	print_list(char *path, t_file *list);
void	print_permissions(unsigned long st_mode);
void	sort_list(t_file **list);

#endif
