/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvan-bre <lvan-bre@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 20:02:50 by lvan-bre          #+#    #+#             */
/*   Updated: 2025/05/16 05:11:34 by lvan-bre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.functions.h"

void	so_long_minishell(t_shell *data, int exitstat)
{
	ft_str_reset(&data->prompt);
	if (data->envp)
		ft_freedarray(data->envp);
	if (data->mlst)
		freemlst(data->mlst);
	if (data->line)
		ft_str_reset(&data->line);
	if (data->pwd)
		ft_str_reset(&data->pwd);
	if (data->old_pwd)
		ft_str_reset(&data->old_pwd);
	if (data->ut)
	{
		reinit_ut(data->ut);
		free(data->ut);
	}
	if (data->path)
		ft_freedarray(data->path);
	rl_clear_history();
	ft_printf("Exit\n");
	exit(exitstat);
}

void	so_long_exec(t_shell *data, int exitstat, char *msg)
{
	if (msg)
		free(msg);
	free(data->prompt);
	if (data->envp)
		ft_freedarray(data->envp);
	if (data->mlst)
		freemlst(data->mlst);
	if (data->line)
		ft_str_reset(&data->line);
	if (data->pwd)
		ft_str_reset(&data->pwd);
	if (data->old_pwd)
		ft_str_reset(&data->old_pwd);
	if (data->ut)
	{
		reinit_ut(data->ut);
		free(data->ut);
	}
	if (data->path)
		ft_freedarray(data->path);
	rl_clear_history();
	exit(exitstat);
}
