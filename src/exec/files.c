/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   files.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvan-bre <lvan-bre@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 01:37:45 by lvan-bre          #+#    #+#             */
/*   Updated: 2025/05/15 21:24:33 by lvan-bre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.functions.h"

bool	open_inf(char *hd, t_rdir in_rdir, t_mlst *lst, t_ctn *ctn)
{
	t_arg	*current;
	int		fd[2];

	current = lst->inf;
	while (current)
	{
		ft_close(&ctn->inf_fd);
		ctn->inf_fd = open(current->ctn, O_RDONLY);
		if (ctn->inf_fd == -1)
			return (perror(current->ctn), false);
		current = current->next;
	}
	if (in_rdir == SING)
		dup2(ctn->inf_fd, STDIN_FILENO);
	else if (in_rdir == DOUB && lst->cmd)
	{
		if (pipe(fd) == -1)
			return (false);
		write(fd[WRITE], hd, ft_strlen(hd));
		close(fd[WRITE]);
		dup2(fd[READ], STDIN_FILENO);
		close(fd[READ]);
	}
	ft_close(&ctn->inf_fd);
	return (true);
}

static bool	open_outf(t_rdir last_rdir, t_rdir type, t_arg *outf, t_ctn *ctn)
{
	t_arg	*current;
	int		mode;

	mode = O_APPEND;
	if (type == SING)
		mode = O_TRUNC;
	current = outf;
	while (current)
	{
		ft_close(&ctn->outf_fd);
		ctn->outf_fd = open(current->ctn, O_WRONLY | O_CREAT | mode, 0666);
		if (ctn->outf_fd == -1)
			return (perror(current->ctn), false);
		current = current->next;
	}
	if (type != last_rdir)
		ft_close(&ctn->outf_fd);
	return (true);
}

bool	open_allfiles(t_mlst *lst)
{
	if (lst->rdir[INF] != NUL)
	{
		if (!open_inf(lst->hd, lst->rdir[INF], lst, lst->ctn))
			return (false);
	}
	if (lst->rdir[OUT] == SING)
	{
		if (!open_outf(lst->rdir[OUT], DOUB, lst->append, lst->ctn))
			return (false);
		if (!open_outf(lst->rdir[OUT], SING, lst->trunc, lst->ctn))
			return (false);
	}
	else if (lst->rdir[OUT] == DOUB)
	{
		if (!open_outf(lst->rdir[OUT], SING, lst->trunc, lst->ctn))
			return (false);
		if (!open_outf(lst->rdir[OUT], DOUB, lst->append, lst->ctn))
			return (false);
	}
	return (true);
}
