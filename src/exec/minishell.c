/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvan-bre <lvan-bre@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 23:27:36 by lvan-bre          #+#    #+#             */
/*   Updated: 2025/05/21 06:09:25 by lvan-bre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.functions.h"

/*
	Waits for all child processes in the linked list to finish.
	Updates exitstatus based on termination or signal status.
	Prints "quit" if a process was terminated by a signal.
	Frees each pid node after waiting.
	Resets the pid pointer at the end.
*/
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
			ft_putendl_fd("Quit", STDERR_FILENO);
		}
		else
			*exitstatus = WEXITSTATUS(status);
		free(*pid);
		*pid = next;
	}
	pid = NULL;
}

/*
	Counts the number of elements in the linked list of arguments.
	Iterates through each node until the end is reached.
	Returns the total count as an integer.
*/
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

/*
	Iterates through the linked list to copy argument strings into an array.
	Skips empty strings and stops if none are found initially.
	Duplicates each valid string; on failure, frees the entire array.
	Sets the last element of the array to NULL to mark its end.
*/
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

/*
	Converts a linked list of arguments into a NULL-terminated array of strings.
	Allocates memory for the array based on the list length.
	Calls a helper to duplicate each argument string into the array.
	Returns the array or NULL on allocation failure.
*/
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

/*
	Main loop of the minishell execution.
	Handles built-in commands without pipes directly.
	Otherwise, executes commands in the list sequentially.
	Resets duplicated input and waits for all child processes.
*/
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
