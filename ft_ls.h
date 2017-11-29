/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asarandi <asarandi@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/25 20:05:13 by asarandi          #+#    #+#             */
/*   Updated: 2017/11/27 20:57:58 by asarandi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_LS_H
# define FT_LS_H

# include <dirent.h> //opendir(), readdir(), closedir()
# include <sys/stat.h> //stat(), lstat()
# include <sys/types.h> //getpwuid()
# include <pwd.h> //getpwuid()
# include <grp.h> //getgrgit()
# include <uuid/uuid.h> //getpwuid(), getgrgid()
# include <sys/xattr.h>  //listxattr(), getxattr()
# include <time.h> //time(), ctime(),
# include <unistd.h> //readlink(), write()
# include <stdlib.h> //malloc(), free(), exit()
# include <stdio.h> //perror()
# include <string.h> //strerror()
# include <termios.h> //tcgetattr
# include <sys/ioctl.h> //ioctl
# include <errno.h>
# include "libft/libft.h"
# include "libft/ft_printf/libftprintf.h"
# include <sys/acl.h> //acl_get_file(), acl_free()

#endif
