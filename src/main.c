/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abarrio- <abarrio-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 14:07:24 by abarrio-          #+#    #+#             */
/*   Updated: 2023/12/21 17:02:29 by abarrio-         ###   ########.fr       */
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
	data->envp = envp;
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

int	wait_childs(t_data *data)
{
	pid_t pid;
	int		status;
	int		final_status;
	
	status = 0;
	final_status = 0;
	while (1)
	{
		pid = waitpid(-1, &status, 0);
		if (pid == -1)
			break ;
		else if (pid == data->ids)
			final_status = status;
	}
	return (final_status);
}
	//fd[0] - read
	//fd[1] - write
	
int	main(int argc, char *argv[], char *envp[])
{
	t_data data;
	int fd[2];

	if (envp && argv && argc > 5)
		pipex_bonus(argc, argv, envp);
	if (!envp || !argv || argc != 5)
		return (0);
	init_stack_pipex(&data, envp, 2, argc);
	if (pipe(fd) < 0)
		exit (EXIT_FAILURE);
	first_child(&data, fd, argv);
	last_child(&data, fd, argv);
	close(fd[1]);
	close(fd[0]);
	//dprintf(2,"uwu\n");para imprimir en la salida de eerrores de la terminal
	return (wait_childs(&data));
}
//zsh: command not found: dnqwi || perror("zsh")
// UwU: no such file or directory: dqdu