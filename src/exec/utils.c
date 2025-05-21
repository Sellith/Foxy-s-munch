/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvan-bre <lvan-bre@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 03:37:31 by lvan-bre          #+#    #+#             */
/*   Updated: 2025/05/21 06:05:51 by lvan-bre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.functions.h"

/*
	Creates a new t_pid node with the given pid.
	Allocates memory and initializes fields.
	Returns NULL if allocation fails.
	Returns pointer to the new node otherwise.
*/
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

/*
	Adds a new t_pid node to the end of the pid list.
	If the list is empty, sets new as the head.
	Otherwise, iterates to the last node and appends new.
	Does nothing if new is NULL.
*/
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

/*
	This function checks if a command matches a builtin.
	It compares the command string with known builtin names.
	Returns the corresponding builtin enum if matched.
	Returns BINARY if no builtin is matched.
*/
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

/*
	This function redirects input/output for a child process in a pipeline.
	It uses /dev/null if input redirection is missing but the previous
	command outputs and if last infile failed to open.
	Output is redirected to a file if specified, otherwise to the pipe.
	Closes all unused file descriptors to avoid leaks.
*/
static void	close_n_dup_child(t_mlst *lst, t_ctn *ctn, int p_fd[2])
{
	int	nullfd;

	close(p_fd[READ]);
	if ((lst->rdir[INF] == NUL && lst->prev && lst->prev->rdir[OUT] != NUL)
		|| (lst->prev && lst->prev->ctn->inf_fd == -1))
	{
		nullfd = open("/dev/null", O_RDWR);
		dup2(nullfd, STDIN_FILENO);
		ft_close(&nullfd);
	}
	if (lst->rdir[OUT] != NUL)
	{
		dup2(ctn->outf_fd, STDOUT_FILENO);
		ft_close(&ctn->outf_fd);
	}
	else if (!lst->lstcmd)
		dup2(p_fd[WRITE], STDOUT_FILENO);
	close(p_fd[WRITE]);
}

/*
	Manage pipe file descriptors based on process role.
	Child process calls close_n_dup_child().
	Parent closes write end of pipe.
	If no output redirection and no next command, dup read end to stdin.
	Close read end of pipe in parent.
*/
void	close_n_dup(t_mlst *lst, t_ctn *ctn, int p_fd[2], int tmpid)
{
	if (tmpid == 0)
		return (close_n_dup_child(lst, ctn, p_fd));
	close(p_fd[WRITE]);
	if (lst->rdir[OUT] == NUL && !lst->lstcmd)
		dup2(p_fd[READ], STDIN_FILENO);
	close(p_fd[READ]);
}
