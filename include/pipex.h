/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abarrio- <abarrio-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 14:07:43 by abarrio-          #+#    #+#             */
/*   Updated: 2023/12/18 07:26:45 by abarrio-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "../libft/ft_printf.h"
# include "../libft/libft.h"

typedef	struct	s_data
{
	pid_t	id;
	char	**command;
}				t_data;

#endif