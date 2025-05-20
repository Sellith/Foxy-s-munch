/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sellith <sellith@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 03:37:31 by lvan-bre          #+#    #+#             */
/*   Updated: 2025/05/17 21:01:57 by sellith          ###   ########.fr       */
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

static void	close_n_dup_child(t_mlst *lst, t_ctn *ctn, int p_fd[2])
{
	int	nullfd;

	nullfd = open("/dev/null", O_RDWR);
	close(p_fd[READ]);
	if ((lst->rdir[INF] == NUL && lst->prev && lst->prev->rdir[OUT] != NUL)
		|| (lst->prev && lst->prev->ctn->inf_fd == -1))
		dup2(nullfd, STDIN_FILENO);
	if (lst->rdir[OUT] != NUL)
	{
		dup2(ctn->outf_fd, STDOUT_FILENO);
		ft_close(&ctn->outf_fd);
	}
	else if (!lst->lstcmd)
		dup2(p_fd[WRITE], STDOUT_FILENO);
	close(p_fd[WRITE]);
	ft_close(&nullfd);
}

void	close_n_dup(t_mlst *lst, t_ctn *ctn, int p_fd[2], int tmpid)
{
	if (tmpid == 0)
		return (close_n_dup_child(lst, ctn, p_fd));
	close(p_fd[WRITE]);
	if (lst->rdir[OUT] == NUL && !lst->lstcmd)
		dup2(p_fd[READ], STDIN_FILENO);
	close(p_fd[READ]);
}
