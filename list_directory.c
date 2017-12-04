/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_directory.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asarandi <asarandi@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/03 18:36:29 by asarandi          #+#    #+#             */
/*   Updated: 2017/12/03 18:57:49 by asarandi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

int		is_allowed(char *fn)
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
