/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asarandi <asarandi@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/25 20:02:16 by asarandi          #+#    #+#             */
/*   Updated: 2017/11/27 04:21:46 by asarandi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"


void	print_file_type(unsigned long st_mode)
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

void	print_file_permissions(unsigned long st_mode)
{
	(st_mode & S_IRUSR) ? write(1, "r", 1) : write(1, "-", 1);
	(st_mode & S_IWUSR) ? write(1, "w", 1) : write(1, "-", 1);
	(st_mode & S_IXUSR) ? write(1, "x", 1) : write(1, "-", 1);
	(st_mode & S_IRGRP) ? write(1, "r", 1) : write(1, "-", 1);
	(st_mode & S_IWGRP) ? write(1, "w", 1) : write(1, "-", 1);
	(st_mode & S_IXGRP) ? write(1, "x", 1) : write(1, "-", 1);
	(st_mode & S_IROTH) ? write(1, "r", 1) : write(1, "-", 1);
	(st_mode & S_IWOTH) ? write(1, "w", 1) : write(1, "-", 1);
	(st_mode & S_IXOTH) ? write(1, "x", 1) : write(1, "-", 1);
}


void print_file_attributes(unsigned long st_mode)
{
	(void)print_file_type(st_mode);
	print_file_permissions(st_mode);
}


int	main(int ac, char **av)
{
	DIR				*dirp;
	struct	dirent	*dp;
	struct	stat	st;
	struct	passwd	*pw;
	struct	group	*gr;

	char	*path;
	if ((ac > 1) && (av[1]))
		path = av[1];
	else
		path = ".";

	dirp = opendir(path); //return NULL on error
	while ((dp = readdir(dirp)) != NULL)
	{
/*
 		ft_printf("dp->d_fileno: %llu\n", dp->d_fileno);
		ft_printf("dp->d_seekoff: %llu\n", dp->d_seekoff);
		ft_printf("dp->d_reclen: %hu\n", dp->d_reclen);
		ft_printf("dp->d_namlen: %hu\n", dp->d_namlen);
		ft_printf("dp->d_type: %hhu\n", dp->d_type);
		ft_printf("dp->d_name: %s\n\n", dp->d_name);
*/	

		char *tmpname = ft_strjoin(path, "/");
		char *newname = ft_strjoin(tmpname, dp->d_name);
		free(tmpname);

		if (stat(newname, &st) != -1)
		{
		print_file_attributes(st.st_mode);
			if ((pw = getpwuid(st.st_uid)) != NULL)
				ft_printf("\t%s", pw->pw_name);
			if ((gr = getgrgid(st.st_gid)) != NULL)
				ft_printf("\t%s", gr->gr_name);
		}
		else
			ft_printf("ft_ls: %s: %s\n", dp->d_name, strerror(errno));
		ft_printf("\t%s\n", dp->d_name);

		free(newname);
	}

	closedir(dirp); //returns 0 on success, -1 on failure

}
