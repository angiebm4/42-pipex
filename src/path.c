/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abarrio- <abarrio-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 10:06:01 by abarrio-          #+#    #+#             */
/*   Updated: 2023/12/22 16:08:53 by abarrio-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

void	*free_split(char **str, char **str1)
{
	int	i;

	i = 0;
	while (str[i])
	{
		free(str[i]);
		i++;
	}
	str = NULL;
	free(str);
	if (str1)
	{
		i = 0;
		while (str1[i])
		{
			free(str1[i]);
			i++;
		}
		str1 = NULL;
		free(str1);
	}
	return (NULL);
}

int	is_full_path(t_data *data)
{
	if (data->command[0][0] == '/')
		return (1);
	if (!ft_strncmp(data->command[0], "../", 3))
		return (1);
	if (!ft_strncmp(data->command[0], "./", 2))
		return (1);
	return (0);
}

static char	*build_path(t_data *data)
{
	int		i;
	char	*path;

	i = 0;
	if (data->path_list == NULL || is_full_path(data) == 1)
		path = ft_strdup(*data->command);
	else
	{
		while (data->path_list[i])
		{
			path = ft_strdup(data->path_list[i]);
			if (!path)
				return (free_split(data->path_list, data->command));
			path = ft_strjoin(path, "/");
			if (!path)
				return (free_split(data->path_list, data->command));
			path = ft_strjoin(path, data->command[0]);
			if (!path)
				return (free_split(data->path_list, data->command));
			if (access(path, F_OK) == 0)
				break ;
			i++;
		}
	}
	return (path);
}

char	*get_path(char *argv, t_data *data)
{
	char	*path;

	data->command = ft_split(argv, ' ');
	if (!data->command)
		return (free_split(data->path_list, NULL));
	path = build_path(data);
	if (!path)
		ft_error_pipex();
	return (path);
}
