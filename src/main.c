/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abarrio- <abarrio-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 14:07:24 by abarrio-          #+#    #+#             */
/*   Updated: 2023/12/27 14:13:39 by abarrio-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

// void	leaks(void)
// {
// 	system("leaks -q pipex");
// }

void	ft_error_pipex(void)
{
	perror("uwu");
	exit(STDERR_FILENO);
}

void	init_stack_pipex(t_data *data, char *envp[], int nb_commands, int argc)
{
	int	i;

	data->argc = argc;
	data->command = NULL;
	data->heredo = 0;
	data->child = 3;
	data->envp = envp;
	i = 0;
	if (envp[0] == NULL)
		data->path_list = NULL;
	else
	{
		while (envp[i] != NULL)
		{
			if (!ft_strncmp(envp[i], "PATH=", 5))
				break ;
			i++;
		}
		data->path_list = ft_split(&envp[i][5], ':');
		if (!data->path_list)
			ft_error_pipex();
	}
	data->nb_commands = nb_commands;
}

int	wait_childs(t_data *data)
{
	pid_t	pid;
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

int	main(int argc, char *argv[], char *envp[])
{
	t_data	data;
	int		fd[2];
	int		status;

	if (envp && argv && argc > 5)
		pipex_bonus(argc, argv, envp);
	if (!envp || !argv || argc != 5)
	{
		perror("put more arguments");
		exit(STDERR_FILENO);
	}
	init_stack_pipex(&data, envp, 2, argc);
	if (pipe(fd) < 0)
		ft_error_pipex();
	first_child(&data, fd, argv);
	last_child(&data, fd, argv);
	close(fd[1]);
	close(fd[0]);
	status = wait_childs(&data);
	exit(status);
}
