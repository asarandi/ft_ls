/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   listdir_norme.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asarandi <asarandi@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/04 03:22:19 by asarandi          #+#    #+#             */
/*   Updated: 2017/12/04 03:47:15 by asarandi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void	parent_node(t_file **head, t_file **index, t_file *new)
{
	(*head) = new;
	(*index) = *head;
}

void	child_node(t_file **index, t_file *node)
{
	(*index)->next = node;
	(*index) = (*index)->next;
}

void	listdir_sortprint(t_file **list, char *path, t_file **start)
{
	sort_list(list);
	if (g_opt.long_list == 0)
		print_basic(*list);
	else
		print_long(path, *list);
	(*start) = *list;
}

void	listdir_nextsubdir(char **subdir)
{
	ft_printf(1, "\n%s:\n", *subdir);
	directory_add_slash(subdir);
	list_directory(*subdir);
}
