/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvan-bre <lvan-bre@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 08:33:35 by azhao             #+#    #+#             */
/*   Updated: 2025/05/21 04:42:33 by lvan-bre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.functions.h"

/*
	This function do getcwd and return an malloced buffer if sucessfull or
	the given str if it fails.
 */
static char	*get_cwd(char *current)
{
	char	*buffer;

	buffer = getcwd(NULL, PATH_MAX);
	if (errno != 0)
		return (current);
	return (buffer);
}

/*
	This function inits data->pwd and init PWD in the envp if needed.
	It returns true if sucessfull and false if there is a malloc error.
 */
static bool	init_cwd(t_shell *data, char ***envp)
{
	char	*buffer;
	int		i;

	i = 0;
	while ((*envp)[i] && ft_strncmp((*envp)[i], "PWD=", 4) != 0)
		i++;
	if (!(*envp)[i])
	{
		data->pwd = get_cwd(data->pwd);
		buffer = ft_strjoin("PWD=", data->pwd);
		if (!buffer)
			return (false);
		*envp = ft_addtoda(*envp, buffer);
	}
	else
		data->pwd = ft_strdup(((*envp)[i]) + 4);
	return (true);
}

/*
	This function inits data->old_pwd and leave it at NULL if the var is not
	found.
 */
static void	init_old_cwd(t_shell *data, char **envp)
{
	int	i;

	i = search_env_var(data->envp, "OLDPWD=");
	if (i != -1)
	{
		if (data->old_pwd)
			free(data->old_pwd);
		data->old_pwd = ft_strdup((envp[i]) + 7);
	}
}

/*
	This function initializes PWD and OLDPWD in the shell.
	If envp exists, it sets both using existing values.
	If not, it gets the current directory and exports it as PWD.
	Returns false if any step fails (e.g., allocation, export), true otherwise.
 */
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
		do_export(&data->envp, buffer);
		ft_str_reset(&buffer);
		if (!data->envp)
			return (false);
		data->old_pwd = NULL;
	}
	return (true);
}

/*
	This function is the executing of ft_pwd it just prints data->pwd
	The function returns an exit code of 1 if there is a write error
	and 2 if it has an option.
 */
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
