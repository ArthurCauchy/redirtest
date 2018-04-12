/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acauchy <acauchy@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/11 11:55:40 by acauchy           #+#    #+#             */
/*   Updated: 2018/04/12 13:53:39 by acauchy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

static void exit_error(char *errmsg)
{
	fprintf(stderr, "errmsg\n");
	exit(1);
}

/*
** role :
** 1 : redirect input
** 2 : redirect ouput
*/

static int	start_process(char *path, int inputfd, int outputfd)
{
	pid_t	pid;
	int		status;
	char	*args[2] = {strdup(path), NULL};

	if ((pid = fork()) == -1)
		exit_error("fork() error");
	if (pid == 0)
	{
		if (inputfd != 0)
		{
			if (close(0) == -1)
				exit_error("close(stdin) error");
			dup(inputfd);
		}
		if (outputfd != 1)
		{
			if (close(1) == -1)
				exit_error("close(stdout) error");
			dup(outputfd);
		}
		execve(path, args, NULL);
	}
	else
	{
		wait(&status);
		if (close(inputfd) == -1)
			exit_error("close(inputfd) error");
		if (close(outputfd) == -1)
			exit_error("close(outputfd) error");
	}
	return (pid);
}

int			main(void)
{
	pid_t	cat_pid;
	int		file_input_fd;
	int		file_output_fd;

	if ((file_input_fd = open("input.txt", O_RDONLY)) == -1)
		exit_error("open() error");
	if ((file_output_fd = open("output.txt", O_WRONLY | O_CREAT, 0755)) == -1)
		exit_error("open() error");
	cat_pid = start_process("/bin/cat", file_input_fd, file_output_fd);
	return (0);
}
