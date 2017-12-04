/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_list.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asarandi <asarandi@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/03 18:26:01 by asarandi          #+#    #+#             */
/*   Updated: 2017/12/03 18:54:03 by asarandi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

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
