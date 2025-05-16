/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvan-bre <lvan-bre@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 03:37:31 by lvan-bre          #+#    #+#             */
/*   Updated: 2025/05/16 05:31:29 by lvan-bre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.functions.h"

t_pid	*new_pid(int new_pid)
{
	t_pid	*pids;

	pids = ft_calloc(1, sizeof(t_pid));
	if (!pids)
		return (NULL);
	pids->pid = new_pid;
	pids->next = NULL;
	return (pids);
}

void	talk_back(t_pid **pid, t_pid *new)
{
	t_pid	*current;

	if (!new)
		return ;
	if (!*pid)
	{
		*pid = new;
		return ;
	}
	current = *pid;
	while (current->next)
		current = current->next;
	current->next = new;
}

t_btins	is_b_in(char *cmd)
{
	t_btins	rt;
	int		cmdlen;

	cmdlen = ft_strlen(cmd);
	rt = BINARY;
	if (ft_strncmp(cmd, "exit", 5) == 0)
		rt = BT_EXIT;
	else if (ft_strncmp(cmd, "echo", 5) == 0)
		rt = BT_ECHO;
	else if (ft_strncmp(cmd, "env", 4) == 0)
		rt = BT_ENV;
	else if (ft_strncmp(cmd, "cd", 3) == 0)
		rt = BT_CD;
	else if (ft_strncmp(cmd, "pwd", 4) == 0)
		rt = BT_PWD;
	else if (ft_strncmp(cmd, "export", 7) == 0)
		rt = BT_EXPORT;
	else if (ft_strncmp(cmd, "unset", 6) == 0)
		rt = BT_UNSET;
	return (rt);
}
