/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sellith <sellith@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 23:25:15 by lvan-bre          #+#    #+#             */
/*   Updated: 2025/05/19 18:36:28 by sellith          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.functions.h"

static int	export_env(char **env)
{
	int		i;

	i = 0;
	if (!env && (*env)[0])
		return (0);
	while (env[i])
	{
		if (ft_strncmp(env[i], "?=", 2) == 0)
			i++;
		if (!env[i])
			return (0);
		if ((write(STDOUT_FILENO, "export", 6)) == -1)
			return (perror(EXPORT_WRITE_ERR), 1);
		if (write(STDOUT_FILENO, env[i], ft_strlen(env[i])) == -1)
			return (perror(EXPORT_WRITE_ERR), 1);
		if ((write(STDOUT_FILENO, "\n", 1)) == -1)
			return (perror(EXPORT_WRITE_ERR), 1);
		i++;
	}
	return (0);
}

static bool	right_args(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (false);
	if (!str[0] || (str[0] != '_' && !ft_isalpha(str[0])))
		return (false);
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (false);
		i++;
	}
	return (true);
}

static bool	conditions(char *str)
{
	char	*tmp;
	char	*buffer;

	if (!right_args(str))
	{
		tmp = ft_strndup(str, ft_strlen_til_char(str, '='));
		buffer = ft_strjoin(IDENTIFIER_ERR, tmp);
		ft_printf("%e", buffer);
		return (ft_freeall("%s%s", tmp, buffer), false);
	}
	return (true);
}

int	do_export(char ***envp, char *str)
{
	int		strlen;
	int		i;

	i = 0;
	if (!conditions(str))
		return (1);
	if (!ft_strchr(str, '='))
		return (0);
	if (!*envp)
		*envp = ft_addtoda(NULL, str);
	strlen = ft_strlen_til_char(str, '=');
	while ((*envp)[i] && ft_strncmp((*envp)[i], str, strlen) != 0)
		i++;
	if (!(*envp)[i])
		*envp = ft_addtoda(*envp, str);
	else
	{
		free((*envp)[i]);
		(*envp)[i] = ft_strdup(str);
	}
	return (0);
}

int	ft_export(char	***envp, char **cmd)
{
	int	i;
	int	res;

	if (ft_darraylen(cmd) == 1)
		return (export_env(*envp));
	else
	{
		i = 1;
		while (cmd[i])
		{
			res = do_export(envp, cmd[i]);
			if (res == 1)
				return (1);
			i++;
		}
	}
	return (0);
}

// int	main(int argc, char **argv, char **envp)
// {
// 	char	**my_array;

// 	(void)argc;
// 	my_array = ft_arraydup(envp);
// 	if (ft_export(&my_array, argv) == 1)
// 		return (1);
// 	ft_printf("%D", my_array);
// 	ft_freedarray(my_array);
// 	return (0);
// }
