/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abarrio- <abarrio-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 09:47:58 by abarrio-          #+#    #+#             */
/*   Updated: 2023/12/21 14:26:01 by abarrio-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

char	*get_heredo(char *delimiter)
{
	char	*aux;
	int		fd;
	
	aux = NULL;
	fd = open("/tmp/pipex_heredo", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (!fd)
		exit (0);
	delimiter = ft_strjoin(ft_strdup(delimiter), "\n");
	if (!delimiter)
		return (NULL);
	while (ft_strcmp(delimiter, aux))
	{
		if (aux)
		{
			write(fd, aux, ft_strlen(aux));
			free(aux);
		}
		aux = get_next_line(0);
		if (!aux)
			break ;
	}
	if (aux)
		free(aux);
	close(fd);
	return("/tmp/pipex_heredo");
}

void	heredo_process(t_data *data, char *argv[])
{
	char	*path;
	int		fd[2];
	int		fd2[2];
	
	data->heredo = 1;
	path = get_heredo(argv[2]);
	if (pipe(fd) < 0)
		exit (EXIT_FAILURE);
	first_child(data, fd, argv);
	if ((data->argc - 2) > data->child)
	{
		if (pipe(fd2) < 0)
			exit (EXIT_FAILURE); // cerrar archivos y hacer osasa de exit o fallos
		mid_child(data, fd, fd2, argv);
		close(fd[0]);
		close(fd[1]);
		data->child += 1;
		while (data->child > 3 && data->child < (data->argc - 2))
		{
			if (pipe(fd) < 0)
				exit (EXIT_FAILURE);
			mid_child(data, fd2, fd, argv);
			fd2[0] = fd[0];
			fd2[1] = fd[1];
			close(fd[0]);
			close(fd[1]);
			data->child += 1;
		}
		
	}
	if (data->argc > 6)
	{
		last_child(data, fd2, argv);
		close(fd2[0]);
		close(fd2[1]);
	}
	else
	{
		last_child(data, fd, argv);
		close(fd[0]);
		close(fd[1]);
	}
	
	unlink(data->path_heredo);
}

void	pipex_bonus(int argc, char *argv[], char *envp[])
{
	t_data	data;
	int fd[2];
	int	fd2[2];
	
	if (!ft_strcmp("here_doc", argv[1]))
	{
		init_stack_pipex(&data, envp, argc - 4, argc);
		heredo_process(&data, argv);
	}
	else
	{
		init_stack_pipex(&data, envp, argc - 3, argc);
		if (pipe(fd) < 0)
			exit (EXIT_FAILURE);
		first_child(&data, fd, argv);
		if ((argc - 2) > data.child)
		{
			if (pipe(fd2) < 0)
				exit (EXIT_FAILURE); // cerrar archivos y hacer osasa de exit o fallos
			mid_child(&data, fd, fd2, argv);
			close(fd[0]);
			close(fd[1]);
			data.child += 1;
			while (data.child > 3 && data.child < (argc - 2))
			{
				if (pipe(fd) < 0)
					exit (EXIT_FAILURE);
				mid_child(&data, fd2, fd, argv);
				fd2[0] = fd[0];
				fd2[1] = fd[1];
				close(fd[0]);
				close(fd[1]);
				data.child += 1;
			}
		}
		last_child(&data, fd2, argv);
	}
	close(fd2[0]);
	close(fd2[1]);
	wait_childs(&data);
	exit (EXIT_SUCCESS);
}

