/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   err_msg.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvan-bre <lvan-bre@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 23:17:37 by lvan-bre          #+#    #+#             */
/*   Updated: 2025/05/22 01:53:05 by lvan-bre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.functions.h"

/*
	Handles error codes after exec attempts.
	Sets shell exit status based on error type (e.g., 127 for not found).
	Prints custom message or perror output.
	Frees command array and exits with status.
*/
void	errhdlg(t_shell *data, t_ctn *ctn, unsigned long err)
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
