/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvan-bre <lvan-bre@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 02:12:12 by lvan-bre          #+#    #+#             */
/*   Updated: 2025/05/22 21:06:35 by lvan-bre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.functions.h"

/*
	This function updates the old path replacing it with the former data->pwd.
	It also export the OLDPWD into the envp.
	It return an exit code of 1 if the code encounters an malloc error.
 */
static unsigned long	update_old_path(t_shell *data)
{
	char	*buffer;

	ft_str_replace(&data->old_pwd, data->pwd);
	if (!data->old_pwd)
		return (1);
	buffer = ft_strjoin("OLDPWD=", data->pwd);
	if (!buffer)
		return (1);
	do_export(&data->envp, buffer);
	if (!data->envp)
		return (1);
	ft_str_reset(&buffer);
	return (0);
}

/*
	This function updates the path changing the path with chdir puting the new
	path update data->pwd with the new path and puting it in the envp.

	Should the directory change be impossible or a malloc error happen this
	function will return an exit code of 1.
 */
static unsigned long	update_path(char *newpwd, t_shell *data)
{
	char	*buffer;

	if (!newpwd)
		return (1);
	buffer = ft_strjoin(CD_STD_ERR, newpwd);
	if (!buffer)
		return (0);
	if (chdir(newpwd) == -1)
		return (perror(buffer), ft_freeall("%s%s", &buffer, &newpwd), 1);
	ft_freeall("%s%s", &buffer, &newpwd);
	if (update_old_path(data))
		return (1);
	ft_str_reset(&data->pwd);
	data->pwd = getcwd(NULL, PATH_MAX);
	if (!data->pwd)
		return (1);
	buffer = ft_strjoin("PWD=", data->pwd);
	if (!buffer)
		return (1);
	do_export(&data->envp, buffer);
	ft_str_reset(&buffer);
	if (!data->envp)
		return (1);
	return (0);
}

/*
	This function expends empty arg or tilde args to home.
	If arg is empty it only looks in the HOME var and returns an error if not
	found.
	If arg is tilde it will look at data->home and return NULL if not found.
 */
static char	*tilde_to_home(t_shell *data, char *cmd)
{
	int	i;

	if (!cmd)
	{
		i = search_env_var(data->envp, "HOME=");
		if (i == -1)
			return (ft_printf("%e", CD_NO_HOME_ERR), NULL);
	}
	if (data->home && *data->home)
		return (ft_strdup(data->home));
	return (NULL);
}

/*
	back_off - Handles relative path resolution starting with '../'.
	Truncates the current working directory accordingly, and appends
	the remaining part of the destination path.
	Returns the resolved path or NULL on failure.
*/
static char	*back_off(t_shell *data, char *dest)
{
	char	*buff;
	int		i;

	buff = ft_strndup(data->pwd, ft_revstrlen_til_char(data->pwd, '/'));
	if (dest[2] == '/' && dest[3])
	{
		i = 3;
		while (dest[i] && (dest[i] == '.' || dest[i] == '/'))
		{
			if (dest[i] == '.' && dest[i + 1] == '.')
			{
				buff = ft_strnfdup(buff, ft_revstrlen_til_char(buff, '/'));
				i += 2;
			}
			if (dest[i] != '/')
				break ;
			else
				i++;
		}
		buff = ft_strdjoining(buff, "/", dest + i);
	}
	return (buff);
}

/*
	This function prepares the executing of ft_cd puting checking for cd's args
	and puting it in buffer in function of if the str needs to be modified to
	be sent to cd's executing function -> "update path".

	Should the cmd have more than 1 args it will result as an error and return
	an exit code of 2.
 */
unsigned long	ft_cd(char **cmd, t_shell *data)
{
	char	*buffer;

	if (ft_darraylen(cmd) <= 2)
	{
		if (cmd[1] && cmd[1][0] == '-')
		{
			if (search_env_var(data->envp, "OLDPWD=") == -1 && !data->old_pwd)
				return (ft_printf("%e", CD_OLD_NOT_SET_ERR), 1);
			ft_printf("%s\n", data->old_pwd);
			return (update_path(ft_strdup(data->old_pwd), data));
		}
		else
		{
			if (ft_darraylen(cmd) == 1 || cmd[1][0] == '~')
				buffer = tilde_to_home(data, cmd[1]);
			else if (cmd[1][0] == '.' && cmd[1][1] == '.')
				buffer = back_off(data, cmd[1]);
			else if (cmd[1][0] != '/')
				buffer = ft_strdjoin(data->pwd, "/", cmd[1]);
			else
				buffer = ft_strdup(cmd[1]);
			return (update_path(buffer, data));
		}
	}
	return (ft_printf("%e", CD_ARGS_ERR), 1);
}
