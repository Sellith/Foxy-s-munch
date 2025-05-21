/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initall.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvan-bre <lvan-bre@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 17:56:55 by sellith           #+#    #+#             */
/*   Updated: 2025/05/21 23:07:08 by lvan-bre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.functions.h"

/*
	Updates the special local variable $? with the last command's exit status:
	- Searches for the variable starting with "?="
	- Converts exit status to string and constructs the new variable string
	- If $? exists, replaces it; otherwise adds it to envp
	- Manages memory allocations and frees intermediate strings
*/
void	last_cmd_status(t_shell *data)
{
	char	*buffer;
	char	*tmp;
	int		i;

	i = 0;
	while (data->envp[i] && ft_strncmp(data->envp[i], "?=", 2) != 0)
		i++;
	tmp = ft_ultoa(data->exitstatus);
	buffer = ft_strjoin("?=", tmp);
	ft_str_reset(&tmp);
	if (!buffer)
		return ;
	if (!data->envp[i])
		data->envp = ft_addtoda(data->envp, buffer);
	else
	{
		free(data->envp[i]);
		data->envp[i] = ft_strdup(buffer);
	}
	free(buffer);
}

/*
	Resets the shell data structure for a new command input:
	- Frees the prompt string
	- Frees and nullifies the command list if it exists
	- Reinitializes the utility sub-structure
	- Resets line size and pipe count to zero
*/
void	reset_data(t_shell *data, t_mlst **lst)
{
	ft_freeall("%s%s", &data->prompt, &data->line);
	if (*lst)
		freemlst(*lst);
	*lst = NULL;
	reinit_ut(data->ut);
	data->lsize = 0;
	data->pipes = 0;
}

/*
	Initializes the main shell data structure by zeroing all fields,
	then allocates memory for the utility sub-structure `ut`.
	Returns true on successful allocation, false otherwise.
*/
bool	initdata(t_shell *data)
{
	ft_bzero(data, sizeof(t_shell));
	data->ut = ft_calloc(1, sizeof(t_utils));
	if (!data->ut)
		return (false);
	return (true);
}

/*
	Frees temporary buffers in the t_utils struct if they exist,
	resets their pointers to NULL, then zeroes out the entire struct.
	Ensures the utility structure is fully cleared and safe for reuse.
*/
void	reinit_ut(t_utils *ut)
{
	ft_freeall("%s%s", &ut->tmpc, &ut->buff);
	ft_bzero(ut, sizeof(t_utils));
}

/*
	Initializes a new t_mlst structure by zeroing its memory and
	allocating an empty t_ctn structure for the content pointer.
	Returns true on successful allocation, false otherwise.
*/
bool	initmlst(t_mlst **new)
{
	ft_bzero((*new), sizeof(t_mlst));
	(*new)->ctn = ft_calloc(1, sizeof(t_ctn));
	if (!(*new)->ctn)
		return (false);
	return (true);
}
