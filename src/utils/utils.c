/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvan-bre <lvan-bre@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 17:35:59 by lvan-bre          #+#    #+#             */
/*   Updated: 2025/05/15 21:24:33 by lvan-bre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.functions.h"

char	**get_path(char **envp)
{
	char	**path;
	int		i;

	i = 0;
	while (envp[i] && ft_strncmp(envp[i], "PATH=", 5) != 0)
		i++;
	if (!envp[i])
		return (ft_addtoda(NULL, ""));
	path = ft_split(envp[i] + 5, ':');
	return (path);
}

void	ft_close(int *fd)
{
	if (*fd > 2)
	{
		close(*fd);
		*fd = -2;
	}
}

void	closefds(int fd[2], int pid)
{
	if (pid == 0)
		close(fd[READ]);
	else
		close(fd[WRITE]);
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
