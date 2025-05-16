/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvan-bre <lvan-bre@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 08:33:35 by azhao             #+#    #+#             */
/*   Updated: 2025/05/16 03:42:05 by lvan-bre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.functions.h"

static char	*get_cwd(char *current)
{
	char	*buffer;

	buffer = getcwd(NULL, PATH_MAX);
	if (errno != 0)
		return (current);
	return (buffer);
}

static bool	init_cwd(t_shell *data, char ***envp)
{
	char	*buffer;
	int		i;

	i = 0;
	while ((*envp)[i] && ft_strncmp((*envp)[i], "PWD=", 4) != 0)
		i++;
	if (!(*envp)[i] && !data->pwd)
	{
		data->pwd = get_cwd(data->pwd);
		buffer = ft_strjoin("PWD=", data->pwd);
		*envp = ft_addtoda(*envp, buffer);
	}
	else if (!(*envp)[i] && data->pwd)
		;
	else
	{
		if (data->pwd)
			free(data->pwd);
		data->pwd = ft_strdup((*envp)[i]);
	}
	return (true);
}

static void	init_old_cwd(t_shell *data, char **envp)
{
	int	i;

	i = 0;
	while (envp[i] && ft_strncmp(envp[i], "OLDPWD=", 7) != 0)
		i++;
	if (!envp[i] && !data->old_pwd)
		data->old_pwd = ft_strdup("");
	else if (!envp[i] && data->old_pwd)
		;
	else
	{
		if (data->old_pwd)
			free(data->old_pwd);
		data->old_pwd = ft_strdup(envp[i]);
	}
}

bool	init_pwd(t_shell *data)
{
	char	*buffer;

	buffer = NULL;
	if (data->envp)
	{
		if (!init_cwd(data, &data->envp))
			return (false);
		init_old_cwd(data, data->envp);
	}
	else
	{
		data->pwd = get_cwd(data->pwd);
		buffer = ft_strjoin("PWD=", data->pwd);
		data->envp = ft_addtoda(data->envp, buffer);
		if (!data->envp)
			return (false);
		data->old_pwd = ft_strdup("");
		if (!data->old_pwd)
			return (false);
	}
	return (true);
}

int	ft_pwd(t_shell *data, char **cmd)
{
	if (cmd[1] && cmd[1][0] == '-')
		return (ft_printf("%e", PWD_OPT_ERR), 2);
	if (write(STDOUT_FILENO, data->pwd, ft_strlen(data->pwd)) == -1)
		return (perror("Foxy's munch: pwd: write error"), 1);
	if (write(STDOUT_FILENO, "\n", 1) == -1)
		return (perror("Foxy's munch: pwd: write error"), 1);
	return (0);
}
