/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   err_msg.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sellith <sellith@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 23:17:37 by lvan-bre          #+#    #+#             */
/*   Updated: 2025/05/20 04:09:40 by sellith          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.functions.h"

void	errhdlg(t_shell *data, t_ctn *ctn, int err)
{
	char	*msg;

	msg = NULL;
	if (err == 2)
	{
		data->exitstatus = 127;
		msg = ft_strjoin(ctn->cmd_ctn[0], NOTF_ERR);
		ft_printf("%e", msg);
	}
	else
	{
		if (err == 1 || err == 13)
			data->exitstatus = 126;
		else if (err == 3)
			data->exitstatus = 128;
		else if (err >= 4)
			data->exitstatus = 128 + err;
		perror(ctn->cmd_ctn[0]);
	}
	ft_freeall("%d", &ctn->cmd_ctn);
	so_long_exec(data, data->exitstatus, msg);
}
