/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abarrio- <abarrio-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 14:07:24 by abarrio-          #+#    #+#             */
/*   Updated: 2023/12/18 08:38:33 by abarrio-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"
#include <stdio.h>
#include <fcntl.h>

void	*free_split(char **str, char **str1)
{
	int	i;
	
	i = 0;
	while(str[i])
	{
		free(str[i]);
		i++;
	}
	str = NULL;
	free(str);
	if (str1)
	{
		i = 0;
		while(str1[i])
		{
			free(str1[i]);
			i++;
		}
		str1 = NULL;
		free(str1);
	}
	return(NULL);
}

char	*build_path(char **path_list, t_data *data)
{
	int		i;
	char	*path;
	
	i = 0;
	while (path_list)
	{
		path = ft_strdup(path_list[i]);
		if (!path)
			return(free_split(path_list, data->command));
		path = ft_strjoin(path, "/");
		if (!path)
			return(free_split(path_list, data->command));
		path = ft_strjoin(path, data->command[0]);
		if (!path)
			return(free_split(path_list, data->command));
		if (access(path, F_OK) == 0)
			break ;
		i++;
	}
	return (path);
}

char	*get_path(char **envp, char *argv, t_data *data)
{
	char	**path_list;
	int		i;
	char	*path;
	
	i = 0;
	while(envp[i] != NULL)
	{
		if (!ft_strncmp(envp[i], "PATH=", 5))
			break ;
		i++;
	}
	path_list = ft_split(&envp[i][5], ':');
	if (!path_list)
		return(NULL);
	data->command = ft_split(argv, ' ');
	if (!data->command)
		return(free_split(path_list, NULL));
	path = build_path(path_list, data);
	free_split(path_list, NULL); //me podria guardar el comando en una variable para no tener que sacarlo otra vez 
	// data->id = fork();
	// if (data->id == 0)
	// 	execve(path, command, envp);
	return (path);
}

// argv 0 es ./pipex
// argv 1 es el infile 
// argv 2 y 3 son comandos 
// arggv 4 outfile 
void	init_stack(t_data *data)
{
	data->id = 0;
	data->command = NULL;
}

int	main(int argc, char *argv[], char *envp[])
{	
	t_data	data;
	char	*path;
	int		fd[2];
	int		infile;
	int		outfile;
	//fd[0] - read
	//fd[1] - write

	if (!envp || !argv || argc != 5)
		return (0);
	init_stack(&data);
	pipe(fd);

	int	ids[2];
	ids[0] = fork();
	if (ids[0] == 0)
	{
		infile = open(argv[1], O_RDONLY);
		if (infile == -1)
			return (0);
		dup2(infile, STDIN_FILENO);
		dup2(fd[1], STDOUT_FILENO);
		path = get_path(envp, argv[2], &data);
		

		close(fd[0]);
		close(fd[1]);
		close(infile);
		
		/*printf("=> Path: [%s]\n=> Arguments:\n", path);
		int index = 0;
		while (data.command[index])
			printf("\t[%s]\n", data.command[index++]);
		printf("\t[NULL]\n=> Env: [%p]\n", envp);*/
		
		execve(path, data.command, envp);
		exit(EXIT_FAILURE);
	}
	ids[1] = fork();
	if (ids[1] == 0)
	{
		outfile = open(argv[argc - 1],  O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (outfile == -1)
			return (0);
		dup2(fd[0], STDIN_FILENO);
		dup2(outfile, STDOUT_FILENO);

		close(fd[0]);
		close(fd[1]);
		close(outfile);

		path = get_path(envp, argv[3], &data);

		/*printf("=> Path: [%s]\n=> Arguments:\n", path);
		int index = 0;
		while (data.command[index])
			printf("\t[%s]\n", data.command[index++]);
		printf("\t[NULL]\n=> Env: [%p]\n", envp);*/
		execve(path, data.command, envp);
		exit(EXIT_FAILURE);
	}
	//while (waitpid(-1, NULL, 0) == -1) // hay que hacer algun bucle para esperar a todos los hijos que devuelve waitpid?? ?
	// dprintf(2, "uwu\n"); para imprimir en la salida de eerrores de la terminal
	
	close(fd[0]);
	close(fd[1]);

	int i;
	i = 0;
	while(i < 2)
	{
		printf("Esperando proceso %d\n", i);
		while (waitpid(ids[i], NULL, 0) == -1)
			;
		printf("Terminado proceso %d\n\n", i);
		i++;
	}

	// int	index = 0;
	// while (envp[index])
	// 	printf("> %s\n", envp[index++]);


	// printf("Buenas\n");
	// char path[] = "/bin/cat";
	// char *arguments[] = {
	// 	"cat",
	// 	"-e",
	// 	"infile",
	// 	NULL
	// };
	// execve(path, arguments, envp);
	
	return (0);
}