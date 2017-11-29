/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asarandi <asarandi@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/25 20:02:16 by asarandi          #+#    #+#             */
/*   Updated: 2017/11/28 18:32:48 by asarandi         ###   ########.fr       */
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

	if ((dirp = opendir(path)) == NULL)
		return (ft_printf("ft_ls: %s: %s\n", path, strerror(errno)));
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

		int	len = ft_strlen(path);
		char *tmpname;
		char *newname;
		if (path[len - 1] != '/')
		{
			tmpname = ft_strjoin(path, "/");
			newname = ft_strjoin(tmpname, dp->d_name);
			free(tmpname);
		}
		else
			newname = ft_strjoin(path, dp->d_name);

		if (lstat(newname, &st) != -1)
		{
			print_file_mode(st.st_mode);
			print_extended_attributes(newname);
			ft_printf(" % 3d", st.st_nlink);
/*
** the number of hard links has to be printed with width specifies
** must be aligned to the right
**    1
**   12
**  372
**  must leave 2 spaces after rightmost permissions/ext-attr character
*/
			if ((pw = getpwuid(st.st_uid)) != NULL)
				ft_printf(" %s", pw->pw_name);
			if ((gr = getgrgid(st.st_gid)) != NULL)
				ft_printf("  %s", gr->gr_name);
			ft_printf("  % 5lu", st.st_size);  /////////// width padding to the right
//			long last_mod;
//		   	last_mod = st.st_mtimespec.tv_sec;
			char *time = ctime(&st.st_mtimespec.tv_sec);
			ft_printf(" %.12s", &(time)[4]);

		}
		else
			ft_printf("ft_ls: %s: %s\n", dp->d_name, strerror(errno));
		ft_printf(" %s\n", dp->d_name);

		free(newname);
	}

	closedir(dirp); //returns 0 on success, -1 on failure

}
