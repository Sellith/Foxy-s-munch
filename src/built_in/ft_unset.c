/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvan-bre <lvan-bre@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 16:51:30 by azhao             #+#    #+#             */
/*   Updated: 2025/05/21 04:54:00 by lvan-bre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.functions.h"

/*
	This function counts how many args will there be after unseting.
*/
static int	count_args(t_shell *data, char **cmd)
{
	int	res;
	int	i;

	res = 0;
	i = 0;
	while (cmd[i])
	{
		if (search_env_var(data->envp, cmd[i]) != -1)
			res++;
		i++;
	}
	return (res);
}

/*
	This function checks if the env var set in argument matches one of the
	args to unset.
	It returns "true" if var and cmd[i] matches and "false" if it doesn't
*/
static bool	check_env(char **cmd, char *var, int args)
{
	int	i;

	i = 0;
	while (i < args)
	{
		if (ft_strncmpcat(cmd[i], var, "=", ft_strlen(cmd[i]) + 1) == 0)
			return (true);
		i++;
	}
	return (false);
}

/*
	This function is the core executing of unset creating another 2D array
	avoiding the vars defined in the cmd.
	It also frees the envp to send it back to envp doing a sort of realloc.
	Should there be a malloc error the function returns NULL.
*/
static char	**do_unset(t_shell *data, char **cmd)
{
	char	**buffer;
	int		envplen;
	int		args;

	args = count_args(data, cmd);
	envplen = ft_darraylen(data->envp);
	buffer = ft_calloc(envplen - args + 1, sizeof(char *));
	if (!buffer)
		return (NULL);
	while (data->ut->j < envplen)
	{
		while (check_env(cmd, data->envp[data->ut->j], args))
			data->ut->j++;
		if (data->ut->j == envplen)
			break ;
		buffer[data->ut->i] = ft_strdup(data->envp[data->ut->j++]);
		if (!buffer[data->ut->i])
			return (ft_freeall("%d", &buffer), NULL);
		data->ut->i++;
	}
	buffer[data->ut->i] = NULL;
	ft_freeall("%d", &data->envp);
	return (buffer);
}

/*
	This function prepares the executing of unset checking if there is any
	unwanted option.
	In that case and in the case of a malloc error this function returns an
	exit code of 2 else it returns 0.
*/
int	ft_unset(char **cmd, t_shell *data)
{
	if (!cmd[1])
		return (0);
	if (cmd[1][0] == '-')
		return (ft_printf("%e", UNSET_OPT_ERR), 2);
	reinit_ut(data->ut);
	data->envp = do_unset(data, cmd + 1);
	if (!data->envp)
		return (2);
	return (0);
}
