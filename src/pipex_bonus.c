/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abarrio- <abarrio-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 09:47:58 by abarrio-          #+#    #+#             */
/*   Updated: 2023/12/18 16:39:56 by abarrio-         ###   ########.fr       */
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

void	pipex_bonus(int argc, char *argv[], char *envp[])
{
	t_data	data;
	char	*path;
	int fd[2];
	int i = 0;
	
	if (!ft_strcmp("here_doc", argv[1]))
	{
		init_stack_pipex(&data, envp, argc - 4, argc);
		data.heredo = 1;
		path = get_heredo(argv[2]);
		pipe(fd);
		first_child(&data, fd, argv, envp);
		while ((argc - 2) > data.child + 1)
		{
			pipe(fd);
			mid_child(&data, fd, argv, envp);
			data.child += 1;
		}
		last_child(&data, fd, argv, envp);
		unlink(data.path_heredo);
	}
	else
	{
		i = 0;
		init_stack_pipex(&data, envp, argc - 3, argc);
		pipe(fd);
		first_child(&data, fd, argv, envp);
		while ((argc - 2) > data.child)
		{
			pipe(fd);
			mid_child(&data, fd, argv, envp);
			data.child += 1;
		}
		pipe(fd);
		last_child(&data, fd, argv, envp);
	}
	i = 0;
	while (i < data.nb_commands)
	{
		while (waitpid(data.ids[i], NULL, 0) == -1)
			;
		i++;
	}
	return ;
}