/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abarrio- <abarrio-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 14:07:43 by abarrio-          #+#    #+#             */
/*   Updated: 2023/12/18 15:29:31 by abarrio-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "../libft/ft_printf.h"
# include "../libft/libft.h"
# include "../libft/get_next_line.h"
# include <stdio.h>
# include <fcntl.h>

typedef	struct	s_data
{
	char	**command;
	char	**path_list;
	pid_t	*ids;
	int		nb_commands;
	int		argc;
	int		heredo;
	int		child;
	char	*path_heredo;
}				t_data;

void	pipex_bonus(int argc, char *argv[], char *envp[]);
void	init_stack_pipex(t_data *data, char *envp[], int nb_commands, int argc);
char	*get_path(char *argv, t_data *data);
void	first_child(t_data *data, int *fd, char *argv[], char *envp[]);
void	last_child(t_data *data, int *fd, char *argv[], char *envp[]);
void	mid_child(t_data *data, int *fd, char *argv[], char *envp[]);

#endif