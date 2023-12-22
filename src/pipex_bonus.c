/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abarrio- <abarrio-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 09:47:58 by abarrio-          #+#    #+#             */
/*   Updated: 2023/12/22 12:22:39 by abarrio-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

char	*get_heredo(char *delimiter)
{
	char	*aux;
	int		fd;

	aux = NULL;
	fd = open("/tmp/pipex_heredo", O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (!fd)
		exit(0);
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
	return ("/tmp/pipex_heredo");
}

void	heredo_process(t_data *data, char *argv[])
{
	int	fd[2];
	int	new[2];

	data->heredo = 1;
	data->path_heredo = get_heredo(argv[2]);
	if (pipe(fd) < 0)
		exit(EXIT_FAILURE);
	first_child(data, fd, argv);
	while ((data->argc - 2) > data->child)
	{
		if (pipe(new) < 0)
			exit(EXIT_FAILURE);
		mid_child(data, fd, new, argv);
		close(fd[0]);
		close(fd[1]);
		fd[0] = new[0];
		fd[1] = new[1];
		data->child += 1;
	}
	last_child(data, fd, argv);
	close(new[0]);
	close(new[1]);
	close(fd[0]);
	close(fd[1]);
	unlink(data->path_heredo);
}

void	normal_files(t_data *data, char *argv[])
{
	int		new[2];
	int		fd[2];

	if (pipe(fd) < 0)
		exit(EXIT_FAILURE);
	first_child(data, fd, argv);
	while ((data->argc - 2) > data->child)
	{
		if (pipe(new) < 0)
			exit(EXIT_FAILURE);
		mid_child(data, fd, new, argv);
		close(fd[0]);
		close(fd[1]);
		fd[0] = new[0];
		fd[1] = new[1];
		data->child += 1;
	}
	last_child(data, fd, argv);
	close(new[0]);
	close(new[1]);
	close(fd[0]);
	close(fd[1]);
}

void	pipex_bonus(int argc, char *argv[], char *envp[])
{
	t_data	data;

	if (!ft_strcmp("here_doc", argv[1]))
	{
		init_stack_pipex(&data, envp, argc - 4, argc);
		heredo_process(&data, argv);
	}
	else
	{
		init_stack_pipex(&data, envp, argc - 3, argc);
		normal_files(&data, argv);
	}
	wait_childs(&data);
	exit(EXIT_SUCCESS);
}
