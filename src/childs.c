/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   childs.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abarrio- <abarrio-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 10:10:46 by abarrio-          #+#    #+#             */
/*   Updated: 2023/12/27 11:34:10 by abarrio-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

void	first_child(t_data *data, int *fd, char *argv[])
{
	int		infile;
	char	*path;

	data->ids = fork();
	if (data->ids == 0)
	{
		close(fd[0]);
		if (data->heredo == 1)
			infile = open(data->path_heredo, O_RDONLY);
		else
			infile = open(argv[1], O_RDONLY);
		if (infile == -1)
			ft_error_pipex();
		path = get_path(argv[2 + data->heredo], data);
		if (!path)
			ft_error_pipex();
		dup2(infile, STDIN_FILENO);
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
		close(infile);
		execve(path, data->command, data->envp);
		ft_error_pipex();
	}
	else if (data->ids < 0)
		ft_error_pipex();
}

void	mid_child(t_data *data, int *fd, int *new, char *argv[])
{
	char	*path;

	data->ids = fork();
	if (data->ids == 0)
	{
		close(fd[1]);
		close(new[0]);
		dup2(fd[0], STDIN_FILENO);
		dup2(new[1], STDOUT_FILENO);
		path = get_path(argv[data->child + data->heredo], data);
		close(fd[0]);
		close(new[1]);
		execve(path, data->command, data->envp);
		ft_error_pipex();
	}
	else if (data->ids < 0)
		ft_error_pipex();
}

void	last_child(t_data *data, int *fd, char *argv[])
{
	int		outfile;
	char	*path;

	data->ids = fork();
	if (data->ids == 0)
	{
		close(fd[1]);
		if (data->heredo == 1)
			outfile = open(argv[data->argc - 1], O_WRONLY | O_CREAT | O_APPEND,
					0644);
		else
			outfile = open(argv[data->argc - 1], O_WRONLY | O_CREAT | O_TRUNC,
					0644);
		if (outfile == -1)
			ft_error_pipex();
		dup2(fd[0], STDIN_FILENO);
		dup2(outfile, STDOUT_FILENO);
		path = get_path(argv[data->argc - 2], data);
		close(fd[0]);
		close(outfile);
		execve(path, data->command, data->envp);
		ft_error_pipex();
	}
	else if (data->ids < 0)
		ft_error_pipex();
}
