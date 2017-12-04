/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   symlink.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asarandi <asarandi@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/03 18:50:10 by asarandi          #+#    #+#             */
/*   Updated: 2017/12/03 18:50:14 by asarandi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

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
