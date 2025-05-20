/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sellith <sellith@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 01:14:25 by lvan-bre          #+#    #+#             */
/*   Updated: 2025/05/20 04:07:48 by sellith          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.functions.h"

static int	bff_path(t_shell *data, t_ctn *ctn)
{
	char	*buffer;
	int		pathlen;
	int		errret;
	int		i;

	i = 0;
	pathlen = ft_darraylen(data->path);
	while (i < pathlen)
	{
		buffer = ft_strdjoin(data->path[i], "/", ctn->cmd_ctn[0]);
		execve(buffer, ctn->cmd_ctn, data->envp);
		errret = errno;
		free(buffer);
		if (errret != 2)
			break ;
		i++;
	}
	if (errret == 2)
	{
		execve(ctn->cmd_ctn[0], ctn->cmd_ctn, data->envp);
		errret = errno;
	}
	return (errret);
}

static void	child_proc(t_shell *data, t_ctn *ctn)
{
	int	errret;

	close(data->stdin_clone);
	// if (is_b_in(lst, ctn->cmd_ctn[0]))
	// 	return (execbt(data, ctn->cmd_ctn));
	init_signals(S_DEFAULT);
	if (ctn->cmd_ctn[0][0] == '/' || ctn->cmd_ctn[0][0] == '.')
	{
		execve(ctn->cmd_ctn[0], ctn->cmd_ctn, data->envp);
		errhdlg(data, ctn, errno);
	}
	errret = bff_path(data, ctn);
	errhdlg(data, ctn, errret);
}

void	exec_cmd(t_shell *data, t_mlst *lst, t_ctn *ctn)
{
	int		fd[2];
	int		tmp_pid;

	if (!lst->next)
		lst->lstcmd = true;
	if (!open_allfiles(lst))
	{
		data->exitstatus = 1;
		return ;
	}
	if (!ctn->cmd_ctn)
		return (ft_close(&ctn->inf_fd), ft_close(&ctn->outf_fd));
	if (pipe(fd) == -1)
		return ;
	tmp_pid = fork();
	if (tmp_pid == -1)
		return ((void)close(fd[READ]), (void)close(fd[WRITE]));
	close_n_dup(lst, ctn, fd, tmp_pid);
	if (tmp_pid == 0)
		child_proc(data, ctn);
	talk_back(&data->pid, new_pid(tmp_pid));
	if (lst->rdir[OUT] != NUL)
		ft_close(&ctn->outf_fd);
}
