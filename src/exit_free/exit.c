/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvan-bre <lvan-bre@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 20:02:50 by lvan-bre          #+#    #+#             */
/*   Updated: 2025/07/02 18:14:16 by lvan-bre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.functions.h"

void	so_long_minishell(t_shell *data, unsigned long exitstat)
{
	if (data->mlst)
		freemlst(data->mlst);
	ft_freeall("%s%s%s%s%d%d%s%s", &data->line, &data->pwd, &data->old_pwd,
		&data->home, &data->path, &data->envp, &data->prompt, &data->usr);
	if (data->ut)
	{
		reinit_ut(data->ut);
		free(data->ut);
	}
	rl_clear_history();
	ft_printf("Exit\n");
	exit(exitstat);
}

void	so_long_exec(t_shell *data, unsigned long exitstat, char *msg)
{
	ft_freeall("%s%s%s%s%d%d%s%s%s", &data->line, &data->pwd, &data->old_pwd,
		&data->home, &data->path, &data->envp, &msg, &data->prompt, &data->usr);
	if (data->mlst)
		freemlst(data->mlst);
	if (data->ut)
	{
		reinit_ut(data->ut);
		free(data->ut);
	}
	rl_clear_history();
	exit(exitstat);
}
