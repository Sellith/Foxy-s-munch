/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execbt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvan-bre <lvan-bre@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 22:15:31 by lvan-bre          #+#    #+#             */
/*   Updated: 2025/05/27 00:12:37 by lvan-bre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.functions.h"

/*
	Executes the built-in command based on its type.
	Calls the corresponding function for each built-in.
	Sets the shell's exit status accordingly.
	Closes stdin clone on exit command.
*/
static void	bt_select(t_shell *data, char **cmd, t_btins type, t_mlst *lst)
{
	if (type == BT_ECHO)
		data->exitstatus = ft_echo(cmd);
	else if (type == BT_EXIT)
	{
		if (lst->ctn->inf_fd != -2)
			ft_close(&data->stdin_clone);
		freepids(data->pid);
		data->exitstatus = ft_exit(data, cmd);
	}
	else if (type == BT_ENV)
		data->exitstatus = ft_env(cmd, data->envp);
	else if (type == BT_CD)
		data->exitstatus = ft_cd(cmd, data);
	else if (type == BT_PWD)
		data->exitstatus = ft_pwd(data, cmd);
	else if (type == BT_EXPORT)
		data->exitstatus = ft_export(&data->envp, cmd);
	else if (type == BT_UNSET)
		data->exitstatus = ft_unset(cmd, data);
}

/*
	Initializes execution environment for built-ins.
	Opens all necessary input/output files.
	Saves current stdin and stdout if it's not an exit command.
	Returns false if file opening fails, true otherwise.
*/
static bool	init_bt_exec(t_shell *data, t_mlst *lst, int std[2], t_btins type)
{
	if (!open_allfiles(lst))
	{
		data->exitstatus = 1;
		return (false);
	}
	if (type != BT_EXIT)
	{
		std[0] = dup(STDIN_FILENO);
		std[1] = dup(STDOUT_FILENO);
	}
	return (true);
}

/*
	Restores original stdin and stdout file descriptors.
	Uses saved descriptors in tmp_std array.
	Closes the saved descriptors after restoring.
*/
static void	reset_std(int tmp_std[2])
{
	dup2(tmp_std[0], STDIN_FILENO);
	dup2(tmp_std[1], STDOUT_FILENO);
	ft_freeall("%c%c", &tmp_std[0], &tmp_std[1]);
}

/*
	Executes a built-in command without pipes.
	Initializes standard fds backup and handles output redirection if needed.
	Calls the proper built-in command handler based on type.
	Restores standard fds after execution if not exiting.
	Frees command content and closes cloned stdin fd.
*/
void	execbt(t_shell *data, t_mlst *lst, t_ctn *ctn, t_btins type)
{
	int	tmp_std[2];

	if (ctn->cmd_ctn == NULL)
		return (ft_freeall("%c", &ctn->outf_fd));
	if (data->pipes == 0)
		init_bt_exec(data, lst, tmp_std, type);
	if (ctn->outf_fd > 2 && data->pipes == 0)
	{
		dup2(ctn->outf_fd, STDOUT_FILENO);
		close(ctn->outf_fd);
	}
	bt_select(data, ctn->cmd_ctn, type, lst);
	if (data->pipes == 0 && type != BT_EXIT)
		reset_std(tmp_std);
	ft_darray_reset(&ctn->cmd_ctn);
	ft_close(&data->stdin_clone);
}
