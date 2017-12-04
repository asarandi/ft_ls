/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dir_util.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asarandi <asarandi@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/03 18:41:19 by asarandi          #+#    #+#             */
/*   Updated: 2017/12/03 18:56:58 by asarandi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

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

int		is_directory(char *path)
{
	struct	stat st;

	lstat(path, &st);
	if (S_ISDIR(st.st_mode))
		return (1);
	else
		return (0);
}

int		ok_to_recurse(char *path)
{
	if (ft_strcmp(path, ".") == 0)
		return (0);
	if (ft_strcmp(path, "..") == 0)
		return (0);
	
	return (1);
}
