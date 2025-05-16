/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initall.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvan-bre <lvan-bre@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 17:56:55 by sellith           #+#    #+#             */
/*   Updated: 2025/05/16 00:35:25 by lvan-bre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.functions.h"

void	last_cmd_status(t_shell *data)
{
	char	*buffer;
	char	*tmp;
	int		i;

	i = 0;
	while (data->envp[i] && ft_strncmp(data->envp[i], "?=", 2) != 0)
		i++;
	tmp = ft_itoa(data->exitstatus);
	buffer = ft_strjoin("?=", tmp);
	free(tmp);
	if (!data->envp[i])
		data->envp = ft_addtoda(data->envp, buffer);
	else
	{
		free(data->envp[i]);
		data->envp[i] = ft_strdup(buffer);
	}
	free(buffer);
}

void	reset_data(t_shell *data, t_mlst **lst)
{
	free(data->prompt);
	ft_str_reset(&data->line);
	if (*lst)
		freemlst(*lst);
	*lst = NULL;
	reinit_ut(data->ut);
	data->lsize = 0;
	data->pipes = 0;
}

bool	initdata(t_shell *data)
{
	ft_bzero(data, sizeof(t_shell));
	data->ut = ft_calloc(1, sizeof(t_utils));
	if (!data->ut)
		return (false);
	return (true);
}

void	reinit_ut(t_utils *ut)
{
	if (ut->tmpc)
		free(ut->tmpc);
	ut->tmpc = NULL;
	if (ut->buff)
		free(ut->buff);
	ut->buff = NULL;
	ft_bzero(ut, sizeof(t_utils));
}

bool	initmlst(t_mlst **new)
{
	ft_bzero((*new), sizeof(t_mlst));
	(*new)->ctn = ft_calloc(1, sizeof(t_ctn));
	if (!(*new)->ctn)
		return (false);
	return (true);
}
