/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvan-bre <lvan-bre@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 02:58:22 by lvan-bre          #+#    #+#             */
/*   Updated: 2025/05/22 19:09:50 by lvan-bre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.functions.h"

void	freerdoc(t_shell *data, t_mlst *new)
{
	ft_freeall("%s%s%s%s%s%s%d%d", &data->prompt, &data->line, &data->home,
		&data->pwd, &data->old_pwd, &data->usr, &data->envp, &data->path);
	if (data->mlst)
		freemlst(data->mlst);
	if (new)
		freemlst(new);
	if (data->ut)
	{
		reinit_ut(data->ut);
		free(data->ut);
	}
	rl_clear_history();
}

void	freerpipe(t_shell *data)
{
	ft_freeall("%s%s%d%d%s%s%s%s", &data->prompt, &data->line, &data->envp,
		&data->path, &data->old_pwd, &data->pwd, &data->home, &data->usr);
	if (data->mlst)
		freemlst(data->mlst);
	if (data->ut)
	{
		reinit_ut(data->ut);
		free(data->ut);
	}
	rl_clear_history();
}

void	freelst(t_arg *arg)
{
	t_arg	*tmp;

	tmp = arg;
	while (arg)
	{
		tmp = arg->next;
		ft_freeall("%s%m", &arg->ctn, &arg);
		arg = tmp;
	}
	arg = NULL;
}

void	freemlst(t_mlst *mlst)
{
	t_mlst	*tmp;

	tmp = mlst;
	while (mlst)
	{
		tmp = mlst->next;
		if (mlst->cmd)
			freelst(mlst->cmd);
		if (mlst->eof)
		{
			ft_str_reset(&mlst->hd);
			freelst(mlst->eof);
		}
		if (mlst->inf)
			freelst(mlst->inf);
		if (mlst->append)
			freelst(mlst->append);
		if (mlst->trunc)
			freelst(mlst->trunc);
		ft_freeall("%m%m", &mlst->ctn, &mlst);
		mlst = tmp;
	}
}
