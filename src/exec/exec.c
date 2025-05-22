/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvan-bre <lvan-bre@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 01:14:25 by lvan-bre          #+#    #+#             */
/*   Updated: 2025/05/22 04:31:53 by lvan-bre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.functions.h"

/*
	get_user - Retrieves the current username by executing 'whoami'.
	Uses a pipe and fork to capture the command output.
	Returns a malloc'd string with the username, or NULL on failure.
*/
char	*get_user(void)
{
	char	*user;
	int		fd[2];
	int		pid;

	if (pipe(fd) == -1)
		return (NULL);
	pid = fork();
	if (pid == -1)
		return (ft_freeall("%c%c", &fd[0], &fd[1]), NULL);
	closefds(fd, pid);
	if (pid == 0)
	{
		dup2(fd[WRITE], STDOUT_FILENO);
		ft_close(&fd[WRITE]);
		execve("/bin/whoami", ft_addtoda(NULL, "whoami"), NULL);
	}
	user = get_n_close(fd[READ]);
	user = ft_strnfdup(user, ft_strlen(user) - 1);
	wait(&pid);
	return (user);
}

/*
	Tries to execute command by searching in all PATH directories.
	Builds full path and calls execve for each until success or error
	other than ENOENT.
	If not found, tries execve with original command.
	Returns last errno value after attempts.
*/
static unsigned long	bff_path(t_shell *data, t_ctn *ctn)
{
	unsigned long	errret;
	char			*buffer;
	int				pathlen;
	int				i;

	i = 0;
	pathlen = ft_darraylen(data->path);
	while (i < pathlen)
	{
		buffer = ft_strdjoin(data->path[i], "/", ctn->cmd_ctn[0]);
		execve(buffer, ctn->cmd_ctn, data->envp);
		errret = errno;
		free(buffer);
		if (errret != ENOENT)
			break ;
		i++;
	}
	if (errret == ENOENT)
	{
		execve(ctn->cmd_ctn[0], ctn->cmd_ctn, data->envp);
		errret = errno;
	}
	return (errret);
}

/*
	Frees all pids inside of the child process.
 */
void	freepids(t_pid *pid)
{
	t_pid	*tmp;

	while (pid)
	{
		tmp = pid->next;
		free(pid);
		pid = tmp;
	}
}

/*
	Handles execution inside the child process after fork.
	Sets default signal handling and closes stdin clone.
	Executes builtin commands if detected.
	If command is a path, tries execve and handles errors.
	Otherwise, searches executable in PATH and handles errors.
*/
static void	child_proc(t_shell *data, t_mlst *lst, t_ctn *ctn)
{
	unsigned long	errret;
	t_btins			type;

	init_signals(S_DEFAULT);
	close(data->stdin_clone);
	type = is_b_in(ctn->cmd_ctn[0]);
	if (type != BINARY)
	{
		execbt(data, lst, ctn, type);
		freepids(data->pid);
		so_long_exec(data, data->exitstatus, NULL);
	}
	if (ctn->cmd_ctn[0][0] == '/' || ctn->cmd_ctn[0][0] == '.')
	{
		execve(ctn->cmd_ctn[0], ctn->cmd_ctn, data->envp);
		freepids(data->pid);
		errhdlg(data, ctn, errno);
	}
	errret = bff_path(data, ctn);
	freepids(data->pid);
	errhdlg(data, ctn, errret);
}

/*
	Executes a command in a child process with proper redirections.
	Handles opening files, creating pipes, and forking.
	In child, runs command with redirections set.
	closes unused fds dups to wanted fds and tracks child pids.
*/
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
	if (!ctn->cmd_ctn || !ctn->cmd_ctn[0])
		return (ft_close(&ctn->inf_fd), ft_close(&ctn->outf_fd));
	if (pipe(fd) == -1)
		return ;
	tmp_pid = fork();
	if (tmp_pid == -1)
		return ((void)close(fd[READ]), (void)close(fd[WRITE]));
	close_n_dup(lst, ctn, fd, tmp_pid);
	if (tmp_pid == 0)
		child_proc(data, lst, ctn);
	talk_back(&data->pid, new_pid(tmp_pid));
	if (lst->rdir[OUT] != NUL)
		ft_close(&ctn->outf_fd);
}
