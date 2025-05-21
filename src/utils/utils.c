/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvan-bre <lvan-bre@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 17:35:59 by lvan-bre          #+#    #+#             */
/*   Updated: 2025/05/21 05:05:20 by lvan-bre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.functions.h"

/*
	Extracts the PATH variable from envp and splits it into an array.
	If PATH is not found, returns an array with an empty string.
	Also frees the previous data->path before updating.
 */
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

/*
	Closes the file descriptor if it is valid (> 2)
	and sets it to -2 to mark it as closed.
*/
void	ft_close(int *fd)
{
	if (*fd > 2)
	{
		close(*fd);
		*fd = -2;
	}
}

/*
	Closes one end of the pipe depending on the process.
	Child process (pid == 0) closes read end,
	parent process closes write end.
*/
void	closefds(int fd[2], int pid)
{
	if (pid == 0)
		close(fd[READ]);
	else
		close(fd[WRITE]);
}

/*
	Searches for an environment variable 'var' in envp.
	Returns the index if found, or -1 if not found or var is empty.
*/
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

/*
	Retrieves the HOME directory from envp and sets data->home.
	If HOME is not found, tries to fallback to the home of the
	current working directory if possible.
	Resets previous home string before setting a new one.
*/
void	get_home(t_shell *data, char **envp)
{
	char	*buffer;
	int		len_til_slash;
	int		i;

	ft_str_reset(&data->home);
	i = search_env_var(envp, "HOME=");
	if (i == -1)
	{
		buffer = getcwd(NULL, PATH_MAX);
		if (ft_strlen(buffer) > 5)
		{
			len_til_slash = ft_strlen_til_char(buffer + 6, '/') + 6;
			data->home = ft_strndup(buffer, len_til_slash);
			ft_str_reset(&buffer);
			return ;
		}
		data->home = NULL;
		return ;
	}
	data->home = ft_strdup((envp)[i] + 5);
	return ;
}
