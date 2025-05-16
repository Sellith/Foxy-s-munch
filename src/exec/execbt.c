/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execbt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvan-bre <lvan-bre@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 22:15:31 by lvan-bre          #+#    #+#             */
/*   Updated: 2025/05/16 06:04:25 by lvan-bre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.functions.h"

static void	bt_select(t_shell *data, char **cmd, t_btins type)
{
	if (type == BT_ECHO)
		data->exitstatus = ft_echo(cmd);
	// else if (type == BT_EXIT)
	// 	data->exitstatus = ft_exit();
	else if (type == BT_ENV)
		data->exitstatus = ft_env(cmd, data->envp);
	else if (type == BT_CD)
		data->exitstatus = ft_cd(cmd, data);
	else if (type == BT_PWD)
		data->exitstatus = ft_pwd(data, cmd);
	else if (type == BT_EXPORT)
		data->exitstatus = ft_export(&data->envp, cmd);
	// else if (type == BT_UNSET)
	// 	data->exitstatus = ft_unset();
}

static bool	init_bt_exec(t_shell *data, t_mlst *lst, int tmp_std[2])
{
	if (!open_allfiles(lst))
	{
		data->exitstatus = 1;
		return (false);
	}
	tmp_std[0] = dup(STDIN_FILENO);
	tmp_std[1] = dup(STDOUT_FILENO);
	return (true);
}

static void	reset_std(int tmp_std[2])
{
	dup2(tmp_std[0], STDIN_FILENO);
	dup2(tmp_std[1], STDOUT_FILENO);
	ft_freeall("%c%c", &tmp_std[0], &tmp_std[1]);
}

void	execbt(t_shell *data, t_mlst *lst, t_ctn *ctn, t_btins type)
{
	int	tmp_std[2];

	if (ctn->cmd_ctn == NULL)
		return (ft_freeall("%c", &ctn->outf_fd));
	if (data->pipes == 0)
		init_bt_exec(data, lst, tmp_std);
	if (ctn->outf_fd > 2)
	{
		dup2(ctn->outf_fd, STDOUT_FILENO);
		close(ctn->outf_fd);
	}
	bt_select(data, ctn->cmd_ctn, type);
	if (data->pipes == 0)
		reset_std(tmp_std);
	ft_darray_reset(&ctn->cmd_ctn);
}
