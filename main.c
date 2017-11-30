/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asarandi <asarandi@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/25 20:02:16 by asarandi          #+#    #+#             */
/*   Updated: 2017/11/30 06:43:23 by asarandi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"
//////////////////
typedef	struct	s_file
{
		char 			*name;
		struct stat		st;
		struct s_file	*prev;
		struct s_file	*next;
}				t_file;

void	print_entry_type(unsigned long st_mode)
{
	if (S_ISREG(st_mode))
		write(1, "-", 1);
	else if (S_ISDIR(st_mode))
		write(1, "d", 1);
	else if (S_ISCHR(st_mode))
		write(1, "c", 1);
	else if (S_ISBLK(st_mode))
		write(1, "b", 1);
	else if (S_ISFIFO(st_mode))
		write(1, "p", 1);
	else if (S_ISLNK(st_mode))
		write(1, "l", 1);
	else if (S_ISSOCK(st_mode))
		write(1, "s", 1);
	else
		write(1, "?", 1);
}

void	print_permissions(unsigned long st_mode)
{
	(st_mode & S_IRUSR) ? write(1, "r", 1) : write(1, "-", 1);
	(st_mode & S_IWUSR) ? write(1, "w", 1) : write(1, "-", 1);
	if (st_mode & S_ISUID)
		(st_mode & S_IXUSR) ? write(1, "s", 1) : write(1, "S", 1);
	else
		(st_mode & S_IXUSR) ? write(1, "x", 1) : write(1, "-", 1);
	(st_mode & S_IRGRP) ? write(1, "r", 1) : write(1, "-", 1);
	(st_mode & S_IWGRP) ? write(1, "w", 1) : write(1, "-", 1);
	if (st_mode & S_ISGID)
		(st_mode & S_IXGRP) ? write(1, "s", 1) : write(1, "S", 1);
	else
		(st_mode & S_IXGRP) ? write(1, "x", 1) : write(1, "-", 1);
	(st_mode & S_IROTH) ? write(1, "r", 1) : write(1, "-", 1);
	(st_mode & S_IWOTH) ? write(1, "w", 1) : write(1, "-", 1);
	if (st_mode & S_ISVTX)
		(st_mode & S_IXOTH) ? write(1, "t", 1) : write(1, "T", 1);
	else
		(st_mode & S_IXOTH) ? write(1, "x", 1) : write(1, "-", 1);
}

void print_file_mode(unsigned long st_mode)
{
	(void)print_entry_type(st_mode);
	(void)print_permissions(st_mode);
}

void	print_extended_attributes(char *path, t_file *file)
{
	acl_t	acl;
	char	*fullpath;

	fullpath = ft_strjoin(path, file->name);
	if ((listxattr(fullpath, NULL, 0, XATTR_NOFOLLOW)) > 0)
		write(1, "@", 1);
	else
	{
		acl = acl_get_link_np(fullpath, ACL_TYPE_EXTENDED);
		if ( acl != NULL)
		{
			acl_free(acl);
			write(1, "+", 1);
		}
		else
			write(1, " ", 1);
	}
	free(fullpath);
}
///////////////////
void	get_file_stats(char *path, t_file *file)
{
	char	*fullpath;

	fullpath = ft_strjoin(path, file->name);
	(void)lstat(fullpath, &file->st);
	free(fullpath);
}
/////////////////
t_file	*create_list(char *path)
{
	DIR				*dirp;
	struct dirent	*dp;
	t_file			*first;
	t_file			*index;
	t_file			*new;

	first = NULL;
	if ((dirp = opendir(path)) == NULL)
		return (NULL);
	while ((dp = readdir(dirp)) != NULL)
	{
		new = (t_file*)ft_memalloc(sizeof(t_file));
		new->name = ft_strdup(dp->d_name);
		get_file_stats(path, new);
		if (first == NULL)
		{
			first = new;
			index = first;
		}
		else
		{
			index->next = new;
			index = index->next;
		}
	}
	index->next = NULL;
	closedir(dirp);
	return (first);
}

void	destroy_list(t_file *list)
{
	t_file	*next;

	while (1)
	{
		next = list->next;
		free(list->name);
		free(list);
		if (next == NULL)
			break ;
		list = next;
	}
	return ;
}

int		sort_by_name_asc(t_file *file1, t_file *file2)
{
	return (ft_strcmp(file1->name, file2->name));
}

int		sort_by_name_desc(t_file *file1, t_file *file2)
{
	return (ft_strcmp(file2->name, file1->name));
}

int		sort_by_size_asc(t_file *file1, t_file *file2)
{
	if (file1->st.st_size > file2->st.st_size)
		return (1);
	else
		return (-1);
}

void	sort_list(t_file *list, int (f)(t_file *f1, t_file *f2))
{
	char		*tmp_name;
	struct stat	tmp_st;
	t_file		*current;

	current = list;
	while (current->next != NULL)
	{
		if ((f(current, current->next)) > 0)
		{
			tmp_name = current->name;
			current->name = current->next->name;
			current->next->name = tmp_name;
			tmp_st = current->st;
			current->st = current->next->st;
			current->next->st = tmp_st;
			current = list;
		}
		else
			current = current->next;
	}
}


int		check_extended_attributes(char *path, t_file *list)
{
	char	*fullpath;
	acl_t	acl;

	while (list != NULL)
	{
		fullpath = ft_strjoin(path, list->name);
		if ((listxattr(fullpath, NULL, 0, 0)) > 0)
			return (1);
		acl = acl_get_link_np(fullpath, ACL_TYPE_EXTENDED);
		if ( acl != NULL)
		{
			acl_free(acl);
			return (1);
		}
		free(fullpath);
		list = list->next;
	}
	return (0);
}

int	count_digits(unsigned long long	n)
{
	int	digits;

	if (n == 0)
		return (1);
	digits = 0;
	while (n)
	{
		digits++;
		n /= 10;
	}
	return (digits);
}

nlink_t	max_hard_links(t_file *list)
{
	nlink_t	max;

	max = 0;
	while (list != NULL)
	{
		if (list->st.st_nlink > max)
			max = list->st.st_nlink;
		list = list->next;
	}
	return (max);
}

off_t	max_file_size(t_file *list)
{
	off_t	max;

	max = 0;
	while (list != NULL)
	{
		if (list->st.st_size > max)
			max = list->st.st_size;
		list = list->next;
	}
	return (max);
}

int	max_length_str_owner(t_file *list)
{
	int				k;
	int				max;
	struct passwd	*pwd;

	max = 0;
	while (list != NULL)
	{
		pwd = getpwuid(list->st.st_uid);
		if (pwd != NULL)
		{
			k = ft_strlen(pwd->pw_name);
			if (k > max)
				max = k;
		}
		list = list->next;
	}
	return (max);
}

int	max_length_str_group(t_file *list)
{
	int				k;
	int				max;
	struct group	*grp;

	max = 0;
	while (list != NULL)
	{
		grp = getgrgid(list->st.st_gid);
		if (grp != NULL)
		{
			k = ft_strlen(grp->gr_name);
			if (k > max)
				max = k;
		}
		list = list->next;
	}
	return (max);
}

int	directory_has_charblocks(t_file *list)
{
	while (list != NULL)
	{
		if ((S_ISCHR(list->st.st_mode)) ||	
				(S_ISBLK(list->st.st_mode)))
		{
			return (1);
		}
		list = list->next;
	}
	return (0);
}

int	charblocks_major_max(t_file *list)
{
	int	max;

	max = 0;
	while (list != NULL)
	{
		if ((S_ISCHR(list->st.st_mode)) ||	
				(S_ISBLK(list->st.st_mode)))
		{
			if (major(list->st.st_rdev) > max)
				max = major(list->st.st_rdev);
		}
		list = list->next;
	}
	return (max);
}

int	charblocks_minor_max(t_file *list)
{
	int	max;

	max = 0;
	while (list != NULL)
	{
		if ((S_ISCHR(list->st.st_mode)) ||	
				(S_ISBLK(list->st.st_mode)))
		{
			if (minor(list->st.st_rdev) > max)
				max = minor(list->st.st_rdev);
		}
		list = list->next;
	}
	return (max);
}

blkcnt_t	count_blocks(t_file *list)
{
	blkcnt_t	total;

	total = 0;
	while (list != NULL)
	{
		total += list->st.st_blocks;
		list = list->next;
	}
	return (total);
}

//           v       v
//0123456789abcdef01234567
//Thu Nov 24 18:22:48 1986\n\0
char	*make_time_string(struct timespec ts)
{
	time_t	now;
	char	*result;
	int		i;

	now = time(&now);
	result = ctime(&ts.tv_sec);
	if ((ts.tv_sec + ((365 / 2) * 86400) < now) ||
			(now + ((365 / 2) * 86400) < ts.tv_sec))
	{
		i = 0;
		while (i < 5)
		{
			result[11 + i] = result[19 + i];
			i++;
		}
	}
	return (&result[4]);
}

char	*get_symlink_address(char *path, t_file *list)
{
	char	*symlink;
	char	*fullpath;

	if ((fullpath = ft_strjoin(path, list->name)) == NULL)
		return (NULL);
	if ((symlink = ft_memalloc(1024)) == NULL)
	{
		free(fullpath);
		return (NULL);
	}
	if ((readlink(fullpath, symlink, 1024)) == -1)
	{
		free(symlink);
		free(fullpath);
		return (NULL);
	}
	free(fullpath);
	return (symlink);
}



void	print_list(char *path, t_file *list)
{
	int				links_width;
	int				owner_width;
	int				group_width;
	int				size_width;
	struct passwd	*pwd;
	struct group	*grp;
	char			*symlink;
	int				has_cb;
	int				cbmaj_width;
	int				cbmin_width;

	ft_printf("total %llu\n", count_blocks(list));
	links_width = count_digits(max_hard_links(list));
	owner_width = max_length_str_owner(list);
	group_width = max_length_str_group(list);
	size_width = count_digits(max_file_size(list));
	has_cb = directory_has_charblocks(list);
	if (has_cb)
	{
		cbmaj_width = count_digits(charblocks_major_max(list));
		cbmin_width = count_digits(charblocks_minor_max(list));
		while (size_width > cbmaj_width + 3 + cbmin_width)
			cbmaj_width++;

	}

	while (list != NULL)
	{
		print_file_mode(list->st.st_mode);
		print_extended_attributes(path, list);
		ft_printf(" %*u ", links_width, list->st.st_nlink);
		pwd = getpwuid(list->st.st_uid);
		if (pwd != NULL)
			ft_printf("%-*s  ", owner_width, pwd->pw_name);
		grp = getgrgid(list->st.st_gid);
		if (grp != NULL)
			ft_printf("%-*s  ", group_width, grp->gr_name);

		if (has_cb)
		{
			if ((S_ISCHR(list->st.st_mode)) || (S_ISBLK(list->st.st_mode)))
				ft_printf("%3d, %3d", major(list->st.st_rdev), minor(list->st.st_rdev));
			else
				ft_printf("%*llu", 8, list->st.st_size);
		}
		else
			ft_printf("%*llu", size_width + 1, list->st.st_size);
		ft_printf(" %.12s", make_time_string(list->st.st_mtimespec));
		ft_printf(" %s", list->name);
		if (S_ISLNK(list->st.st_mode))
		{
			symlink = get_symlink_address(path, list);
			if (symlink != NULL)
			{
				ft_printf(" -> %s", symlink);
				free(symlink);
			}
		}
		ft_printf("\n");
		list = list->next;
	}
}


void	list_directory(char *path)
{
	t_file	*list;

	list = create_list(path);
	sort_list(list, sort_by_name_asc);
	print_list(path, list);
	destroy_list(list);
}

int	is_directory(char *path)
{
	DIR	*dirp;

	if ((dirp = opendir(path)) != NULL)
	{
		closedir(dirp);
		return (1);
	}
	return (0);
}

/////////////////
int	main(int ac, char **av)
{
	char	*path;
	char	*tmp;

	if ((ac > 1) && (av[1]))
		path = ft_strdup(av[1]);
	else
		path = ft_strdup(".");

	if (path[ft_strlen(path) - 1] != '/')
	{
		tmp = ft_strjoin(path, "/");
		free(path);
		path = tmp;
	}
	if (is_directory(path))
		list_directory(path);

	return (0);
	
}
