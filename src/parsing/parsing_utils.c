/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvan-bre <lvan-bre@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 11:38:10 by lvan-bre          #+#    #+#             */
/*   Updated: 2025/05/21 05:31:25 by lvan-bre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.functions.h"

/*
	Checks if the character c is one of the shell token characters:
	- '>', '<', space, or pipe '|'.
	Returns true if it is, false otherwise.
*/
bool	is_token(char c)
{
	if (c != '>' && c != '<' && c != ' ' && c != '|')
		return (false);
	return (true);
}

/*
	Checks if the character c is valid for environment variable names:
	- Alphanumeric characters, underscore '_', or question mark '?'.
	Returns true if valid, false otherwise.
*/
bool	is_envable(char c)
{
	if ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z')
		|| (c >= 'A' && c <= 'Z') || c == '_' || c == '?')
		return (true);
	return (false);
}

/*
	Retrieves the content (ctn) of the last argument node in the linked list.
	If the last node has unmatched quotes (dq or sq), marks its q_trace flag as
	true.
	Returns the content string of the last argument.
*/
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

/*
	Updates the shell exit status using the global signal code g_sig.
	Calls last_cmd_status to finalize command status.
	Resets g_sig to zero.
*/
void	exit_sig(t_shell *data)
{
	data->exitstatus = g_sig;
	last_cmd_status(data);
	g_sig = 0;
}
