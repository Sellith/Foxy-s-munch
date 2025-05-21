/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   to_list.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvan-bre <lvan-bre@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 07:38:07 by lvan-bre          #+#    #+#             */
/*   Updated: 2025/05/21 05:10:58 by lvan-bre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.functions.h"

/*
	Scans 'line' to find the first token outside quotes.
	Counts single and double quotes to handle quoting context.
	Returns index of token or -1 on error (empty input or unmatched quotes).
	Updates dt->lsize with scanned length.
*/
static int	quote_check(char *line, int pre_len, t_arg *arg, t_shell *dt)
{
	int	i;

	i = -1;
	while (line[++i])
	{
		if (is_token(line[i]) && (arg->sq % 2 == 0 && arg->dq % 2 == 0))
			break ;
		if (line[i] == '\'' && arg->dq % 2 == 0)
		{
			arg->sq++;
			arg->q = true;
		}
		else if (line[i] == '\"' && arg->sq % 2 == 0)
		{
			arg->dq++;
			arg->q = true;
		}
	}
	if (i == 0)
		return (ft_printf("%s", TOK_ERR), -1);
	if (!line[i] && (arg->sq % 2 == 1 || arg->dq % 2 == 1))
		return (ft_printf("%s", QUOTE_ERR), -1);
	dt->lsize = pre_len + i;
	return (i);
}

/*
	Removes part of dt->line from the new list, then appends 'new' argument node
	to the end of the linked list pointed to by *cmd.
	Initializes *cmd if it was NULL.
*/
static void	arg_bark(t_arg **cmd, t_arg *new, t_shell *dt)
{
	t_arg	*head;

	dt->line = rm_from_line(dt, dt->line);
	if (!*cmd)
	{
		*cmd = new;
		return ;
	}
	head = *cmd;
	while (head->next)
		head = head->next;
	head->next = new;
}

/*
	Allocates and initializes a new argument node from dt->line
	starting at dt->ut->i.
	Extracts the argument content considering quotes, and appends
	the node to *arg.
	Returns true on success, false if allocation or parsing fails.
*/
static bool	split_args(t_shell *dt, t_arg **arg, t_rdir type)
{
	t_arg	*new;

	new = ft_calloc(1, sizeof(t_arg));
	if (!new)
		return (false);
	reinit_ut(dt->ut);
	dt->ut->i = type;
	while (dt->line[dt->ut->i] == ' ')
		dt->ut->i++;
	dt->ut->size = quote_check(dt->line + dt->ut->i, dt->ut->i, new, dt);
	if (dt->ut->size == -1)
		return (free(new), false);
	dt->ut->buff = ft_calloc((dt->ut->size + 1), sizeof(char));
	if (!dt->ut->buff)
		return (free(new), false);
	while (dt->ut->j < dt->ut->size)
		dt->ut->buff[dt->ut->j++] = dt->line[dt->ut->i++];
	dt->ut->buff[dt->ut->j] = '\0';
	dt->ut->j = -1;
	new->ctn = put_ctn(dt->ut->buff, dt->ut, new, dt);
	if (!new->ctn)
		return (free(new), false);
	new->next = NULL;
	arg_bark(arg, new, dt);
	return (true);
}

/*
	Allocates and initializes a new here-document argument node from
	dt->line starting at dt->ut->i.
	Extracts the here-doc content considering quotes, and appends the
	node to *arg.
	Returns true on success, false if allocation or parsing fails.
*/
static bool	split_hd(t_shell *dt, t_arg **arg, t_rdir type)
{
	t_arg	*new;

	new = ft_calloc(1, sizeof(t_arg));
	if (!new)
		return (false);
	reinit_ut(dt->ut);
	dt->ut->i = type;
	while (dt->line[dt->ut->i] == ' ')
		dt->ut->i++;
	dt->ut->size = quote_check(dt->line + dt->ut->i, dt->ut->i, new, dt);
	if (dt->ut->size == -1)
		return (free(new), false);
	dt->ut->buff = ft_calloc((dt->ut->size + 1), sizeof(char));
	if (!dt->ut->buff)
		return (free(new), false);
	while (dt->ut->j < dt->ut->size)
		dt->ut->buff[dt->ut->j++] = dt->line[dt->ut->i++];
	dt->ut->buff[dt->ut->j] = '\0';
	dt->ut->j = -1;
	new->ctn = put_ctn_hd(dt->ut->buff, dt->ut, new, dt);
	if (!new->ctn)
		return (free(new), false);
	new->next = NULL;
	arg_bark(arg, new, dt);
	return (true);
}

/*
	Selects and calls the appropriate split function based on the IO type and
	redirection type:
	- CMD: command arguments
	- INF: input redirection (SING = <, DOUB = <<)
	- OUT: output redirection (SING = >, DOUB = >>)

	Updates the corresponding field in the new mlst structure with the parsed
	argument.

	Returns true on success. On failure, sets exitstatus to 1, frees new, and
	returns false.
*/
bool	select_tolkien(t_shell *dt, t_mlst *new, t_rdir rdir, t_io i_o)
{
	bool	ret;

	ret = false;
	if (i_o == CMD)
		ret = split_args(dt, &new->cmd, rdir);
	else
	{
		if (i_o == INF && rdir == SING)
			ret = split_args(dt, &new->inf, rdir);
		else if (i_o == INF && rdir == DOUB)
			ret = split_hd(dt, &new->eof, rdir);
		else if (i_o == OUT && rdir == SING)
			ret = split_args(dt, &new->trunc, rdir);
		else if (i_o == OUT && rdir == DOUB)
			ret = split_args(dt, &new->append, rdir);
		new->rdir[i_o] = rdir;
	}
	if (!ret)
	{
		dt->exitstatus = 1;
		return (freemlst(new), false);
	}
	return (true);
}
