/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asarandi <asarandi@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/25 20:02:16 by asarandi          #+#    #+#             */
/*   Updated: 2017/12/01 20:42:59 by asarandi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"
//////////////////

char	*g_ls_name;

typedef	struct	list_options {
	int	long_list;		// -l
	int	recursive;		// -R
	int	reverse;		// -r
	int show_dot;		// -a
	int show_hidden;	// -A
	int sort; 			//     (0) by name,
						// -t  (1) by time mod,
						// -u  (2) by time access,
						// -U  (3) by time created,
						// -S  (4) by size
						// -f (-1) no sort
	
	int colors;			// -G
	int hide_owner;		// -g -- turns on l
	int hide_group; 	// -o -- turn on l
	int show_acl;		// -e -- when used with l
	int	show_extattr;	// -@ -- when used with l
	int plain;			// -d
	int last_opt;		// where options end and arguments start

} t_options;

t_options	g_opt;

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
int	get_file_stats(char *path, t_file *file)
{
	char	*fullpath;
	int		result;

	fullpath = ft_strjoin(path, file->name);
	result = lstat(fullpath, &file->st);
	free(fullpath);
	if (result == -1)
		ft_printf(2, "%s: %s: %s\n", g_ls_name, file->name, strerror(errno));
	return (result);
}
/////////////////
int	is_allowed(char *fn)
{
	int	allowed;

	allowed = 1;
	if (fn[0] == '.')
	{	
		allowed = 0;
		if (g_opt.show_hidden == 1)
		{
			if ((ft_strcmp(fn, ".")) == 0)
				allowed = 0;
			else if ((ft_strcmp(fn, "..")) == 0)
				allowed = 0;
			else
				allowed = 1;
		}
		if (g_opt.show_dot == 1)
			allowed = 1;
	}
	return (allowed);
}


t_file	*create_list(char *path)
{
	DIR				*dirp;
	struct dirent	*dp;
	t_file			*first;
	t_file			*index;
	t_file			*new;

	first = NULL;
	index = NULL;
	errno = 0;
	if ((dirp = opendir(path)) == NULL)
		return (NULL);
	while ((dp = readdir(dirp)) != NULL)
	{
		if ((is_allowed(dp->d_name)) == 1)
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
	}
	if (index)
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

//////////////////////////////////////////////////////
int		sort_by_name_asc(t_file *file1, t_file *file2)
{
	return (ft_strcmp(file1->name, file2->name));
}

int		sort_by_name_desc(t_file *file1, t_file *file2)
{
	return (ft_strcmp(file2->name, file1->name));
}
///////////////////////////////////////////////////////
int		sort_by_size_asc(t_file *file1, t_file *file2)
{
	if (file2->st.st_size > file1->st.st_size)
		return (1);
	if (file2->st.st_size < file1->st.st_size)
		return (-1);
	else
		return (sort_by_name_asc(file1, file2));
}

int		sort_by_size_desc(t_file *file1, t_file *file2)
{
	if (file2->st.st_size > file1->st.st_size)
		return (-1);
	if (file2->st.st_size < file1->st.st_size)
		return (1);
	else
		return (sort_by_name_desc(file1, file2));
}
///////////////////////////////////////////////////////
int		sort_by_mtime_asc(t_file *file1, t_file *file2)
{
	if (file2->st.st_mtimespec.tv_sec > file1->st.st_mtimespec.tv_sec)
		return (1);
	else if (file2->st.st_mtimespec.tv_sec < file1->st.st_mtimespec.tv_sec)
		return (-1);
	else if (file2->st.st_mtimespec.tv_nsec > file1->st.st_mtimespec.tv_nsec)
			return (1);
	else if (file2->st.st_mtimespec.tv_nsec < file1->st.st_mtimespec.tv_nsec)
			return (-1);
	else
		return (sort_by_name_asc(file1, file2));
}

int		sort_by_mtime_desc(t_file *file1, t_file *file2)
{
	if (file2->st.st_mtimespec.tv_sec > file1->st.st_mtimespec.tv_sec)
		return (-1);
	else if (file2->st.st_mtimespec.tv_sec < file1->st.st_mtimespec.tv_sec)
		return (1);
	else if (file2->st.st_mtimespec.tv_nsec > file1->st.st_mtimespec.tv_nsec)
			return (-1);
	else if (file2->st.st_mtimespec.tv_nsec < file1->st.st_mtimespec.tv_nsec)
			return (1);
	else
		return (sort_by_name_desc(file1, file2));
}
//////////////////////////////////////////////////////
int		sort_by_atime_asc(t_file *file1, t_file *file2)
{
	if (file2->st.st_atimespec.tv_sec > file1->st.st_atimespec.tv_sec)
		return (1);
	else if (file2->st.st_atimespec.tv_sec < file1->st.st_atimespec.tv_sec)
		return (-1);
	else if (file2->st.st_atimespec.tv_nsec > file1->st.st_atimespec.tv_nsec)
			return (1);
	else if (file2->st.st_atimespec.tv_nsec < file1->st.st_atimespec.tv_nsec)
			return (-1);
	else
		return (sort_by_name_asc(file1, file2));
}

int		sort_by_atime_desc(t_file *file1, t_file *file2)
{
	if (file2->st.st_atimespec.tv_sec > file1->st.st_atimespec.tv_sec)
		return (-1);
	else if (file2->st.st_atimespec.tv_sec < file1->st.st_atimespec.tv_sec)
		return (1);
	else if (file2->st.st_atimespec.tv_nsec > file1->st.st_atimespec.tv_nsec)
			return (-1);
	else if (file2->st.st_atimespec.tv_nsec < file1->st.st_atimespec.tv_nsec)
			return (1);
	else
		return (sort_by_name_desc(file1, file2));
}
//////////////////////////////////////////////////////
int		sort_by_ctime_asc(t_file *file1, t_file *file2)
{
	if (file2->st.st_ctimespec.tv_sec > file1->st.st_ctimespec.tv_sec)
		return (1);
	else if (file2->st.st_ctimespec.tv_sec < file1->st.st_ctimespec.tv_sec)
		return (-1);
	else if (file2->st.st_ctimespec.tv_nsec > file1->st.st_ctimespec.tv_nsec)
			return (1);
	else if (file2->st.st_ctimespec.tv_nsec < file1->st.st_ctimespec.tv_nsec)
			return (-1);
	else
		return (sort_by_name_asc(file1, file2));
}

int		sort_by_ctime_desc(t_file *file1, t_file *file2)
{
	if (file2->st.st_ctimespec.tv_sec > file1->st.st_ctimespec.tv_sec)
		return (-1);
	else if (file2->st.st_ctimespec.tv_sec < file1->st.st_ctimespec.tv_sec)
		return (1);
	else if (file2->st.st_ctimespec.tv_nsec > file1->st.st_ctimespec.tv_nsec)
			return (-1);
	else if (file2->st.st_ctimespec.tv_nsec < file1->st.st_ctimespec.tv_nsec)
			return (1);
	else
		return (sort_by_name_desc(file1, file2));
}
//////////////////////////////////////////////////////
int		sort_by_btime_asc(t_file *file1, t_file *file2)
{
	if (file2->st.st_birthtimespec.tv_sec > file1->st.st_birthtimespec.tv_sec)
		return (1);
	else if (file2->st.st_birthtimespec.tv_sec < file1->st.st_birthtimespec.tv_sec)
		return (-1);
	else if (file2->st.st_birthtimespec.tv_nsec > file1->st.st_birthtimespec.tv_nsec)
			return (1);
	else if (file2->st.st_birthtimespec.tv_nsec < file1->st.st_birthtimespec.tv_nsec)
			return (-1);
	else
		return (sort_by_name_asc(file1, file2));
}

int		sort_by_btime_desc(t_file *file1, t_file *file2)
{
	if (file2->st.st_birthtimespec.tv_sec > file1->st.st_birthtimespec.tv_sec)
		return (-1);
	else if (file2->st.st_birthtimespec.tv_sec < file1->st.st_birthtimespec.tv_sec)
		return (1);
	else if (file2->st.st_birthtimespec.tv_nsec > file1->st.st_birthtimespec.tv_nsec)
			return (-1);
	else if (file2->st.st_birthtimespec.tv_nsec < file1->st.st_birthtimespec.tv_nsec)
			return (1);
	else
		return (sort_by_name_desc(file1, file2));
}
//////////////////////////////////////////////////////








//int cmp(t_file *a, t_file *b) {
//    return (ft_strcmp(a->name,  b->name));
//}

/*
**  merge sort algo by simon tatham,
**  https://www.chiark.greenend.org.uk/~sgtatham/algorithms/listsort.c
*/

t_file *listsort(t_file *list, int (f)(t_file *f1, t_file *f2))
{
    t_file *p, *q, *e, *tail;
    int insize, nmerges, psize, qsize, i;

    if (!list) 	return NULL;
    insize = 1;
    while (1) {
        p = list;
        list = NULL;
        tail = NULL;
        nmerges = 0;  /* count number of merges we do in this pass */
        while (p) {
            nmerges++;  /* there exists a merge to be done */
            q = p;
            psize = 0;
            for (i = 0; i < insize; i++) {
                psize++;
		    q = q->next;
                if (!q) break;
            }
            qsize = insize;
            while (psize > 0 || (qsize > 0 && q)) {
                if (psize == 0) {
		    e = q; q = q->next; qsize--;
		} else if (qsize == 0 || !q) {
		    e = p; p = p->next; psize--;
		} else if (f(p,q) <= 0) {
		    e = p; p = p->next; psize--;
		} else {
		    e = q; q = q->next; qsize--;
		}
		if (tail) {
		    tail->next = e;
		} else {
		    list = e;
		}
		tail = e;
            }
            p = q;
        }
	    tail->next = NULL;
        if (nmerges <= 1)   /* allow for nmerges==0, the empty list case */
            return list;
        insize *= 2;
    }
}





void	sort_list(t_file **list)
{

	int (*f)(t_file *f1, t_file *f2);

	f = sort_by_name_asc;
	if (g_opt.sort == -1)
		return ;
	else if (g_opt.sort == 0)
		f = g_opt.reverse == 0 ? sort_by_name_asc : sort_by_name_desc;

	else if (g_opt.sort == 1)
		f = g_opt.reverse == 0 ? sort_by_mtime_asc : sort_by_mtime_desc;


	else if (g_opt.sort == 2)
		f = g_opt.reverse == 0 ? sort_by_atime_asc : sort_by_atime_desc;


	else if (g_opt.sort == 3)
		f = g_opt.reverse == 0 ? sort_by_btime_asc : sort_by_btime_desc;



	else if (g_opt.sort == 4)
		f = g_opt.reverse == 0 ? sort_by_size_asc : sort_by_size_desc;

//	int sort; 			//     (0) by name,
						// -t  (1) by time mod,
						// -u  (2) by time access,
						// -U  (3) by time created,
						// -S  (4) by size
						// -f (-1) no sort
	(*list) = listsort(*list, f);

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
	if ((symlink = ft_memalloc(PATH_MAX)) == NULL)
	{
		free(fullpath);
		return (NULL);
	}
	if ((readlink(fullpath, symlink, PATH_MAX)) == -1)
	{
		free(symlink);
		free(fullpath);
		return (NULL);
	}
	free(fullpath);
	return (symlink);
}




void print_basic(t_file *list)
{
	while (list)
	{
		printf("%s\n", list->name);
		list = list->next;
	}
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

	if ((list) && (g_opt.long_list == 0))
	{
		print_basic(list);
		return ;
	}

	if (*path)
		ft_printf(1, "total %llu\n", count_blocks(list));
	links_width = count_digits(max_hard_links(list));
	owner_width = max_length_str_owner(list);
	group_width = max_length_str_group(list);
	size_width = count_digits(max_file_size(list));
	has_cb = directory_has_charblocks(list);

	while (list != NULL)
	{
		print_file_mode(list->st.st_mode);
		print_extended_attributes(path, list);
		ft_printf(1, " %*u ", links_width, list->st.st_nlink);
		pwd = getpwuid(list->st.st_uid);
		if ((pwd != NULL) && (g_opt.hide_owner == 0))
			ft_printf(1, "%-*s  ", owner_width, pwd->pw_name);
		grp = getgrgid(list->st.st_gid);
		if ((grp != NULL) && (g_opt.hide_group == 0))
			ft_printf(1, "%-*s  ", group_width, grp->gr_name);
		if ((g_opt.hide_owner == 1) && (g_opt.hide_group == 1))
			ft_printf(1, "  ");
		if (has_cb)
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
				ft_printf(1, "%*s%*llu ", 8 - size_width, "", size_width, list->st.st_size);
		}
		else
			ft_printf(1, "%*llu ", size_width, list->st.st_size);
		ft_printf(1, "%.12s ", make_time_string(list->st.st_mtimespec));
		ft_printf(1, "%s", list->name);
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

void	directory_add_slash(char **path)
{
	int	len;
	char *tmp;

	len = ft_strlen(*path);

	if ((*(*path)) && ((*path)[len - 1] != '/'))
	{
		tmp = ft_strjoin(*path, "/");
		free((*path));
		(*path) = tmp;
	}
}

int	is_directory(char *path)
{
	struct	stat st;

	lstat(path, &st);
	if (S_ISLNK(st.st_mode))
		return (0);
	if (S_ISDIR(st.st_mode))
		return (1);
	else
		return (0);
}

char *directory_add_path(char *path, char *dir)
{
	int		len;
	char	*result;
	char	*tmp;

	len = ft_strlen(path);
	if (path[len - 1] != '/')
		tmp = ft_strjoin(path, "/");
	else
		tmp = path;
	result = ft_strjoin(tmp, dir);
	if (tmp != path)
		free(tmp);
	len = ft_strlen(result);
	if (result[len - 1] != '/')
	{
		tmp = result;
		result = ft_strjoin(result, "/");
		free(tmp);
	}
	return (result);
}


int	ok_to_recurse(char *path)
{
	if (ft_strcmp(path, ".") == 0)
		return (0);
	if (ft_strcmp(path, "..") == 0)
		return (0);
	
	return (1);
}

void	list_directory(char *path)
{
	t_file	*list;
	t_file	*start;
	char	*subdir;

	list = create_list(path);
	if (list != NULL)
	{
		sort_list(&list);
		print_list(path, list);
		start = list;
		if (g_opt.recursive == 1)
		{
			while (list)
			{
				subdir = ft_strjoin(path, list->name);
				if ((is_directory(subdir)) && (ok_to_recurse(list->name)))
				{
					ft_printf(1, "\n%s:\n", subdir);
					directory_add_slash(&subdir);
					list_directory(subdir);
				}
				free(subdir);
				list = list->next;
			}
		}
		destroy_list(start);
	}
	else if (errno)
		ft_printf(2, "%s: %s: %s\n", g_ls_name, path, strerror(errno));
}

void	illegal_option(char c)
{
	ft_printf(2, "%s: illegal option -- %c\n", g_ls_name, c);
	ft_printf(2, "usage: ls [-AadefGgloRrStUu@] [file ...]\n");
	exit(0);
}

void	clear_options()
{
	g_opt.long_list = 0;
	g_opt.recursive = 0;
	g_opt.reverse = 0;
	g_opt.show_dot = 0;
	g_opt.show_hidden = 0;
	g_opt.sort = 0;
	g_opt.colors = 0;
	g_opt.hide_owner = 0;
	g_opt.hide_group = 0;
	g_opt.show_acl = 0;
	g_opt.show_extattr = 0;
	g_opt.plain = 0;
	g_opt.last_opt = 1;
}

void parse_options(int ac, char **av)
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
				else if (av[i][k] == 'R')
					g_opt.recursive = 1;
				else if (av[i][k] == 'r')
					g_opt.reverse = 1;
				else if (av[i][k] == 'a')
					g_opt.show_dot = 1;
				else if (av[i][k] == 'A')
					g_opt.show_hidden = 1;
				else if (av[i][k] == 't')
					g_opt.sort = 1;
				else if (av[i][k] == 'u')
					g_opt.sort = 2;
				else if (av[i][k] == 'U')
					g_opt.sort = 3;
				else if (av[i][k] == 'S')
					g_opt.sort = 4;
				else if (av[i][k] == 'f')
					g_opt.sort = -1;
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

// add error handling in get_file_stats

t_file *build_file_list(int ac, char **av)
{
	int	i;
	t_file			*first;
	t_file			*index;
	t_file			*new;

	i = g_opt.last_opt;
	first = NULL;
	while (i < ac)
	{
		if (is_directory(av[i]) == 0)
		{
			new = (t_file*)ft_memalloc(sizeof(t_file));
			new->name = ft_strdup(av[i]);
			if (get_file_stats("", new) == 0)
			{
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
			else
			{
				free(new->name);
				free(new);
			}
		}
		i++;
	}
	return (first);
}

t_file *build_directory_list(int ac, char **av)
{
	int	i;
	t_file			*first;
	t_file			*index;
	t_file			*new;

	i = g_opt.last_opt;
	first = NULL;
	while (i < ac)
	{
		if (is_directory(av[i]) == 1)
		{
			new = (t_file*)ft_memalloc(sizeof(t_file));
			new->name = ft_strdup(av[i]);
			get_file_stats("", new);
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
		i++;
	}
	return (first);
}


//display_file_list


int	display_files(int ac, char **av)
{
	t_file	*list;

	list = build_file_list(ac, av);
	if (list != NULL)
	{
		sort_list(&list);
		print_list("", list);
		destroy_list(list);
		return (1);
	}
	return (0);

}

void	display_directories(int ac, char **av,  int flag)
{
	t_file	*list;
	t_file	*first;

	list = build_directory_list(ac, av);
	first = list;
	if ((list != NULL) && (flag))
		ft_printf(1, "\n");
//	flag = 0;
	if ((list) && (list->next))
		flag = 1;
	if (list != NULL)
	{
		sort_list(&list);
		while (list)
		{
			if (flag)
				ft_printf(1, "%s:\n", list->name);
			directory_add_slash(&list->name);
			list_directory(list->name);
			list = list->next;
			if (list)
				ft_printf(1, "\n");
		}
		destroy_list(first);
	}
}


/////////////////
int	main(int ac, char **av)
{
	int			mixed_input;

	g_ls_name = av[0];
	if ((av[0][0] == '.') && (av[0][1] == '/'))
		g_ls_name = &av[0][2];
	parse_options(ac, av);
	if (!av[g_opt.last_opt])
	{
		g_opt.last_opt -= 1;
		av[g_opt.last_opt][0] = '.';
		av[g_opt.last_opt][1] = 0;
	}
	mixed_input = display_files(ac, av);
	display_directories(ac, av, mixed_input);
	return (0);
}
