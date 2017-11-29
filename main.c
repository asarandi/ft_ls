/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asarandi <asarandi@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/25 20:02:16 by asarandi          #+#    #+#             */
/*   Updated: 2017/11/28 23:32:03 by asarandi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

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

int	print_extended_attributes(char *filename)
{
	acl_t	acl;

	if ((listxattr(filename, NULL, 0, 0)) > 0)
		return (write(1, "@", 1));
	else
	{
		acl = acl_get_link_np(filename, ACL_TYPE_EXTENDED);
		if ( acl != NULL)
		{
			acl_free(acl);
			return (write(1, "+", 1));
		}
	}
	return (0);
}
//////////////////
typedef	struct	s_file
{
		char 			*name;
		struct stat		st;
		struct s_file	*prev;
		struct s_file	*next;
}				t_file;
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

void	sort_list_by_name_rev(t_file *list)
{
	char		*tmp_name;
	struct stat	tmp_st;
	t_file		*current;

	current = list;
	while (current->next != NULL)
	{
		if ((ft_strcmp(current->next->name, current->name)) > 0)
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








void	print_list(t_file *list)
{
	while (list != NULL)
	{
		ft_printf("%s\n", list->name);
		list = list->next;
	}
}

/////////////////
int	main(int ac, char **av)
{
	char	*path;
	char	*tmp;
	t_file	*list;

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

	list = create_list(path);
	print_list(list);
	sort_list(list, sort_by_name_asc);
	ft_printf("............................\n");
	print_list(list);
	sort_list(list, sort_by_name_desc);
	ft_printf("............................\n");
	print_list(list);

	sort_list(list, sort_by_size_asc);
	ft_printf("............................\n");
	print_list(list);



	destroy_list(list);
	return (0);
	
}
