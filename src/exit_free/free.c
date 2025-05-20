/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sellith <sellith@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 02:58:22 by lvan-bre          #+#    #+#             */
/*   Updated: 2025/05/20 02:12:53 by sellith          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.functions.h"

void	freerdoc(t_shell *data, t_mlst *new)
{
	free(data->prompt);
	if (data->envp)
		ft_freedarray(data->envp);
	if (data->line)
		ft_str_reset(&data->line);
	if (data->mlst)
		freemlst(data->mlst);
	if (new)
		freemlst(new);
	if (data->ut)
	{
		reinit_ut(data->ut);
		free(data->ut);
	}
	if (data->path)
		ft_freedarray(data->path);
	rl_clear_history();
}

void	freerpipe(t_shell *data)
{
	ft_freeall("%s%s%d%d", &data->prompt, &data->line, &data->envp, 
		&data->path);
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
