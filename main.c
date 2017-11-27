/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asarandi <asarandi@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/25 20:02:16 by asarandi          #+#    #+#             */
/*   Updated: 2017/11/26 18:10:07 by asarandi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

int	main(int ac, char **av)
{
	DIR				*dirp;
	struct	dirent	*dp;
	struct termios	termios_p;
	struct winsize	ws;
	int				r;

	r = tcgetattr(STDOUT_FILENO, &termios_p);
	if (r == -1)
		printf("%s\n", strerror(errno));

	if (isatty(1) == 1)
		printf("isatty(1): outputting to a terminal\n");
	else
	{

		printf("isatty(1): not a terminal\nerror: %s\n", strerror(errno));
	}



	printf("TERMIOS (returns %d) c_iflag %lu, c_oflag %lu, c_cflag %lu, c_lflag %lu, c_ispeed %lu, c_ospeed %lu\n", r,
					termios_p.c_iflag, \
					termios_p.c_oflag, \
					termios_p.c_cflag, \
					termios_p.c_lflag, \
					termios_p.c_ispeed, \
					termios_p.c_ospeed );


	r = ioctl(0, TIOCGWINSZ, &ws);
//	ws_row = 0, ws_col = 0, ws_xpixel = 0, ws_ypixel = 0
	printf("TIOCGWINSZ (returns %d) ws_row: %d, ws_col: %d, ws_xpixel: %d, ws_ypixel: %d\n", r,
						ws.ws_row, ws.ws_col, ws.ws_xpixel, ws.ws_ypixel);
	


	dirp = opendir("."); //return NULL on error
	while ((dp = readdir(dirp)) != NULL)
	{
		printf("dp->d_fileno: %llu\n", dp->d_fileno);
		printf("dp->d_seekoff: %llu\n", dp->d_seekoff);
		printf("dp->d_reclen: %hu\n", dp->d_reclen);
		printf("dp->d_namlen: %hu\n", dp->d_namlen);
		printf("dp->d_type: %hhu\n", dp->d_type);
		printf("dp->d_name: %s\n\n", dp->d_name);
	}


	closedir(dirp); //returns 0 on success, -1 on failure

}
