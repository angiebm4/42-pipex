/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   childs.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abarrio- <abarrio-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 10:10:46 by abarrio-          #+#    #+#             */
/*   Updated: 2023/12/18 16:43:42 by abarrio-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"


void	first_child(t_data *data, int *fd, char *argv[], char *envp[])
{
	int		infile;
	char	*path;

	data->ids[0] = fork();
	if (data->ids[0] == 0)
	{
		if (data->heredo == 1)
			infile = open(data->path_heredo, O_RDONLY);
		else 
			infile = open(argv[1], O_RDONLY);
		if (infile == -1)
			exit(EXIT_FAILURE);
		dup2(infile, STDIN_FILENO);
		dup2(fd[1], STDOUT_FILENO);
		path = get_path(argv[2 + data->heredo], data);
		close(fd[0]);
		close(fd[1]);
		close(infile);
		/*printf("=> Path: [%s]\n=> Arguments:\n", path);
		int index = 0;
		while (data.command[index])
			printf("\t[%s]\n", data.command[index++]);
		printf("\t[NULL]\n=> Env: [%p]\n", envp);*/
		execve(path, data->command, envp);
		exit(EXIT_FAILURE);
	}
}

void	mid_child(t_data *data, int *fd, char *argv[], char *envp[])
{
	char	*path;

	data->ids[data->child - 2] = fork();
	if (data->ids[data->child - 2] == 0)
	{
		dup2(fd[0], STDIN_FILENO);
		dup2(fd[1], STDOUT_FILENO);
		path = get_path(argv[data->child + data->heredo], data);
		close(fd[0]);
		close(fd[1]);
		//tiene dos tuberias !!!!!!!!!!
		/*printf("=> Path: [%s]\n=> Arguments:\n", path);
		int index = 0;
		while (data.command[index])
			printf("\t[%s]\n", data.command[index++]);
		printf("\t[NULL]\n=> Env: [%p]\n", envp);*/
		execve(path, data->command, envp);
		exit(EXIT_FAILURE);
	}
}
void	last_child(t_data *data, int *fd, char *argv[], char *envp[])
{
	int		outfile;
	char	*path;

	data->ids[1] = fork();
	if (data->ids[data->nb_commands - 1] == 0)
	{
		outfile = open(argv[data->argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (outfile == -1)
			exit(EXIT_FAILURE);
		dup2(fd[0], STDIN_FILENO);
		dup2(outfile, STDOUT_FILENO);
		close(fd[0]);
		close(fd[1]);
		close(outfile);
		path = get_path(argv[3], data);
		/*printf("=> Path: [%s]\n=> Arguments:\n", path);
		int index = 0;
		while (data.command[index])
			printf("\t[%s]\n", data.command[index++]);
		printf("\t[NULL]\n=> Env: [%p]\n", envp);*/
		execve(path, data->command, envp);
		exit(EXIT_FAILURE);
	}
	close(fd[0]);
	close(fd[1]);
}
