/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   files.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvan-bre <lvan-bre@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 01:37:45 by lvan-bre          #+#    #+#             */
/*   Updated: 2025/05/21 18:18:55 by lvan-bre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.functions.h"

/*
	Opens input files for redirection, setting the input file descriptor.
	For single input redirection, opens files for reading and duplicates fd to
	STDIN.
	For double input (heredoc), creates a pipe and writes heredoc content to it.
	Returns false if any open or pipe call fails, else true.
	Closes file descriptors properly to avoid leaks.
*/
bool	open_inf(char *hd, t_rdir rdir, t_mlst *lst, t_ctn *ctn)
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
	if (rdir == SING && ft_strncmp(ctn->cmd_ctn[0], "exit", 5) != 0)
		dup2(ctn->inf_fd, STDIN_FILENO);
	else if (rdir == DOUB && lst->cmd && ft_strncmp(ctn->cmd_ctn[0], "exit", 5))
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

/*
	Opens output files for redirection with proper mode (append or truncate).
	Iterates over the list of output files, opening each one in write mode.
	Closes previous output file descriptor before opening a new one.
	Returns false and prints error if any open call fails.
	Closes the last output fd if redirection type differs from last redirection.
*/
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

/*
	Opens all input/output files for the given command list node.
	Handles input redirection if specified.
	Opens output files in correct order based on single or double redirection.
		if mode is append open all truncate files then the append files
		if mode is append open all append files then the truncate files
	Returns false if any file opening fails, true otherwise.
*/
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
