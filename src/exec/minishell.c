/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvan-bre <lvan-bre@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 23:27:36 by lvan-bre          #+#    #+#             */
/*   Updated: 2025/05/20 18:56:24 by lvan-bre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.functions.h"

static void	waitall(t_pid **pid, int	*exitstatus)
{
	t_pid	*next;
	int		status;

	while (*pid)
	{
		next = (*pid)->next;
		waitpid((*pid)->pid, &status, 0);
		if (WIFSIGNALED(status))
		{
			*exitstatus = WTERMSIG(status) + 128;
			ft_putendl_fd("quit", STDERR_FILENO);
		}
		else
			*exitstatus = WEXITSTATUS(status);
		free(*pid);
		*pid = next;
	}
	pid = NULL;
}

static int	ft_lstlen(t_arg *lst)
{
	t_arg	*current;
	int		i;

	i = 0;
	current = lst;
	while (current)
	{
		i++;
		current = current->next;
	}
	return (i);
}

static void	cmd_dupcheck(t_arg *current, char ***cmd)
{
	int	i;

	i = 0;
	while (current)
	{
		while (current && !current->ctn[0])
		{
			current = current->next;
			if (!current)
			{
				if (i == 0)
					return (ft_darray_reset(cmd));
				else
					return ;
			}
		}
		(*cmd)[i] = ft_strdup(current->ctn);
		if (!(*cmd)[i])
			return (ft_darray_reset(cmd));
		i++;
		current = current->next;
	}
	(*cmd)[i] = NULL;
}

char	**ft_ltoda(t_arg *cmd)
{
	char	**buffer;
	t_arg	*current;

	current = cmd;
	buffer = ft_calloc(ft_lstlen(current) + 1, sizeof(char *));
	if (!buffer)
		return (NULL);
	cmd_dupcheck(current, &buffer);
	return (buffer);
}

void	minishell(t_shell *data, t_mlst *lst)
{
	t_btins	bt;
	t_mlst	*current;

	current = lst;
	if (lst->cmd)
		bt = is_b_in(current->cmd->ctn);
	else
		bt = BT_NULL;
	if (data->pipes == 0 && bt != BT_NULL && bt != BINARY)
	{
		current->ctn->cmd_ctn = ft_ltoda(current->cmd);
		return (execbt(data, current, current->ctn, bt));
	}
	while (current)
	{
		current->ctn->cmd_ctn = ft_ltoda(current->cmd);
		if (current->ctn->cmd_ctn)
		{
			exec_cmd(data, current, current->ctn);
			ft_darray_reset(&current->ctn->cmd_ctn);
		}
		current = current->next;
	}
	ft_dup_close(data->stdin_clone, STDIN_FILENO);
	waitall(&data->pid, &data->exitstatus);
}
