/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvan-bre <lvan-bre@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 11:38:10 by lvan-bre          #+#    #+#             */
/*   Updated: 2025/05/15 21:24:33 by lvan-bre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.functions.h"

bool	is_token(char c)
{
	if (c != '>' && c != '<' && c != ' ' && c != '|')
		return (false);
	return (true);
}

bool	is_envable(char c)
{
	if ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z')
		|| (c >= 'A' && c <= 'Z') || c == '_' || c == '?')
		return (true);
	return (false);
}

char	*get_last_ctn(t_arg *arg)
{
	t_arg	*head;

	head = arg;
	while (head->next)
		head = head->next;
	if (head->dq || head->sq)
		head->q_trace = true;
	return (head->ctn);
}

void	exit_sig(t_shell *data)
{
	data->exitstatus = g_sig;
	last_cmd_status(data);
	g_sig = 0;
}
