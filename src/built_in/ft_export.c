/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvan-bre <lvan-bre@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 23:25:15 by lvan-bre          #+#    #+#             */
/*   Updated: 2025/05/22 01:21:26 by lvan-bre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.functions.h"

/*
	This function prints the environment in the format of the export command.
	It skips the local variable "?=" since it's a local variable and handles
	write errors.
	Returns 1 on write error, 0 otherwise.
 */
static unsigned long	export_env(char **env)
{
	int		i;

	i = 0;
	if (!env || !(*env)[0])
		return (0);
	while (env[i])
	{
		if (ft_strncmp(env[i], "?=", 2) == 0)
			i++;
		if (!env[i])
			return (0);
		if ((write(STDOUT_FILENO, "export ", 7)) == -1)
			return (perror(EXPORT_WRITE_ERR), 1);
		if (write(STDOUT_FILENO, env[i], ft_strlen(env[i])) == -1)
			return (perror(EXPORT_WRITE_ERR), 1);
		if ((write(STDOUT_FILENO, "\n", 1)) == -1)
			return (perror(EXPORT_WRITE_ERR), 1);
		i++;
	}
	return (0);
}

/*
	This function checks if the given string is a valid variable name for export.
	It must start with a letter or underscore, and contain only alphanumerics
	or underscores before '='.
	Returns true if the name is valid, false otherwise.
 */
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

/*
	This function checks if the export argument is a valid identifier.
	If not, it prints an error message with the invalid name.
	Returns true if the identifier is valid, false otherwise.
 */
static bool	conditions(char *str)
{
	char	*tmp;
	char	*buffer;

	if (!right_args(str))
	{
		tmp = ft_strndup(str, ft_strlen_til_char(str, '='));
		buffer = ft_strjoin(IDENTIFIER_ERR, tmp);
		if (!buffer)
			return (ft_str_reset(&tmp), false);
		ft_printf("%e", buffer);
		return (ft_freeall("%s%s", &tmp, &buffer), false);
	}
	return (true);
}

/*
	This function adds or updates an environment variable in envp.
	It first validates the variable format, then adds it if new,
	or replaces the existing one if the name matches.
	Returns 0 on success, 1 on error.
 */
unsigned long	do_export(char ***envp, char *str)
{
	int		strlen;
	int		i;

	i = 0;
	if (!str || str[0] == '-')
		return (ft_dprintf(STDERR_FILENO, "%s\n", EXP_OPT_ERR), 2);
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
		if (!(*envp)[i])
			return (ft_darray_reset(envp), 1);
	}
	return (0);
}

/*
	This function is the executing of the export built-in command.
	If no arguments are given, it prints the environment in export format.
	Otherwise, it exports each variable and updates the shell's exit status
	on error.
	Returns 0 if sucessfull and 1 if export_env fails
	(errors are stored in data->exitstatus).
 */
unsigned long	ft_export(t_shell *data, char	***envp, char **cmd)
{
	unsigned long	res;
	int				i;

	if (ft_darraylen(cmd) == 1)
		return (export_env(*envp));
	else
	{
		i = 1;
		while (cmd[i])
		{
			res = do_export(envp, cmd[i]);
			if (res != 0)
				data->exitstatus = res;
			i++;
		}
	}
	return (0);
}
