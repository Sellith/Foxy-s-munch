/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvan-bre <lvan-bre@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 02:12:12 by lvan-bre          #+#    #+#             */
/*   Updated: 2025/05/21 03:17:41 by lvan-bre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.functions.h"

static int	update_old_path(t_shell *data)
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
	free(buffer);
	return (0);
}

static int	update_path(char *newpwd, t_shell *data)
{
	char	*buffer;

	if (!newpwd)
		return (1);
	buffer = ft_strjoin(CD_STD_ERR, newpwd);
	if (!buffer)
		return (1);
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
	free(buffer);
	if (!data->envp)
		return (1);
	return (0);
}

static char	*tilde_to_usr(t_shell *data, char *cmd)
{
	int	i;

	if (!cmd)
	{
		i = search_env_var(data->envp, "HOME=");
		if (i == -1)
			return (ft_printf("%e", CD_NO_HOME_ERR), NULL);
	}
	if (data->home)
		return (ft_strdup(data->home));
	ft_printf("%e", CD_NO_HOME_ERR);
	return (NULL);
}

int	ft_cd(char **cmd, t_shell *data)
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
			if (ft_darraylen(cmd) == 1)
				buffer = tilde_to_usr(data, NULL);
			else if (cmd[1][0] != '/' && cmd[1][0] != '~')
				buffer = ft_strdjoin(data->pwd, "/", cmd[1]);
			else if (cmd[1][0] == '~')
				buffer = tilde_to_usr(data, cmd[1]);
			else
				buffer = ft_strdup(cmd[1]);
			return (update_path(buffer, data));
		}
	}
	return (ft_printf("%e", CD_ARGS_ERR), 1);
}
