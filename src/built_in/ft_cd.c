/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvan-bre <lvan-bre@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 02:12:12 by lvan-bre          #+#    #+#             */
/*   Updated: 2025/05/16 05:51:00 by lvan-bre         ###   ########.fr       */
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

	buffer = ft_strjoin(CD_STD_ERR, newpwd);
	if (!buffer)
		return (1);
	if (chdir(newpwd) == -1)
		return (perror(buffer), ft_freeall("%s%s", &buffer, &newpwd), 1);
	ft_str_reset(&newpwd);
	if (update_old_path(data))
		return (1);
	ft_str_reset(&data->pwd);
	data->pwd = getcwd(NULL, PATH_MAX);
	if (!data->pwd)
		return (1);
	ft_printf("%s\n", data->pwd);
	ft_str_reset(&buffer);
	buffer = ft_strjoin("PWD=", data->pwd);
	if (!buffer)
		return (1);
	do_export(&data->envp, buffer);
	free(buffer);
	if (!data->envp)
		return (1);
	return (0);
}

static bool	search_env_var(char **envp, char *var)
{
	int	len;
	int	i;

	len = ft_strlen(var);
	if (len == 0)
		return (false);
	i = 0;
	while (envp[i] && ft_strncmp(envp[i], var, len) != 0)
		i++;
	if (!envp[i])
		return (false);
	return (true);
}

int	ft_cd(char **cmd, t_shell *data)
{
	char	*buffer;
	int		i;

	if (ft_darraylen(cmd) == 2)
	{
		if (cmd[1][0] == '-')
		{
			i = 0;
			if (!search_env_var(data->envp, "OLDPWD="))
				return (ft_printf("%e", CD_OLD_NOT_SET_ERR), 1);
			return (update_path(ft_strdup(data->old_pwd), data));
		}
		else
		{
			if (cmd[1][0] != '/')
				buffer = ft_strdjoin(data->pwd, "/", cmd[1]);
			else
				buffer = ft_strdup(cmd[1]);
			if (!buffer)
				return (1);
			return (update_path(buffer, data));
		}
	}
	ft_printf("%e", CD_ARGS_ERR);
	return (1);
}
