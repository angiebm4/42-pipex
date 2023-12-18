/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abarrio- <abarrio-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 14:07:24 by abarrio-          #+#    #+#             */
/*   Updated: 2023/12/18 16:43:46 by abarrio-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

// argv 0 es ./pipex
// argv 1 es el infile
// argv 2 y 3 son comandos
// arggv 4 outfile

void	init_stack_pipex(t_data *data, char *envp[], int nb_commands, int argc)
{
	int	i;

	data->argc = argc;
	data->command = NULL;
	data->heredo = 0;
	data->child = 3;
	i = 0;
	while (envp[i] != NULL)
	{
		if (!ft_strncmp(envp[i], "PATH=", 5))
			break ;
		i++;
	}
	data->path_list = ft_split(&envp[i][5], ':');
	if (!data->path_list)
	{
		perror("zsh");
		exit(1);
	}

	data->nb_commands = nb_commands;
}
	//fd[0] - read
	//fd[1] - write
	
int	main(int argc, char *argv[], char *envp[])
{
	t_data data;
	int fd[2];
	int status;

	if (envp && argv && argc > 5)
		pipex_bonus(argc, argv, envp);
	if (!envp || !argv || argc != 5)
		return (0);
	init_stack_pipex(&data, envp, 2, argc);
	pipe(fd);
	first_child(&data, fd, argv, envp);
	last_child(&data, fd, argv, envp);
	//dprintf(2,"uwu\n");para imprimir en la salida de eerrores de la terminal
	// close(fd[0]);
	// close(fd[1]);
	int i;
	i = 0;
	while (i < data.nb_commands)
	{
		// printf("Esperando proceso %d\n", i);
		while (waitpid(data.ids[i], &status, 0) == -1)
			;
		// printf("Terminado proceso %d\n\n", i);
		i++;
	}
	return (status);
}
//zsh: command not found: dnqwi || perror("zsh")
// UwU: no such file or directory: dqdu