/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sellith <sellith@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 17:35:59 by lvan-bre          #+#    #+#             */
/*   Updated: 2025/05/20 03:32:51 by sellith          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.functions.h"

char	**get_path(t_shell *data, char **envp)
{
	char	**path;
	int		i;

	i = 0;
	ft_darray_reset(&data->path);
	while (envp[i] && ft_strncmp(envp[i], "PATH=", 5) != 0)
		i++;
	if (!envp[i])
		return (ft_addtoda(NULL, ""));
	path = ft_split(envp[i] + 5, ':');
	return (path);
}

void	ft_close(int *fd)
{
	if (*fd > 2)
	{
		close(*fd);
		*fd = -2;
	}
}

void	closefds(int fd[2], int pid)
{
	if (pid == 0)
		close(fd[READ]);
	else
		close(fd[WRITE]);
}

int	search_env_var(char **envp, char *var)
{
	int	len;
	int	i;

	len = ft_strlen(var);
	if (len == 0)
		return (-1);
	i = 0;
	while (envp[i] && ft_strncmp(envp[i], var, len) != 0)
		i++;
	if (!envp[i])
		return (-1);
	return (i);
}

void	get_home(t_shell *data, char **envp)
{
	char	*buffer;
	int		i;

	ft_str_reset(&data->home);
	i = search_env_var(envp, "HOME=");
	if (i == -1)
	{
		buffer = getcwd(NULL, PATH_MAX);
		if (ft_strlen(buffer) > 5)
		{
			data->home = ft_strndup(buffer, ft_strlen_til_char(buffer + 6, '/') + 6);
			ft_str_reset(&buffer);
			return ;
		}
		data->home = NULL;
		return ;
	}
	data->home = ft_strdup((envp)[i] + 5);
	return ;
}
