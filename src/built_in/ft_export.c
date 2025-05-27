/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvan-bre <lvan-bre@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 23:25:15 by lvan-bre          #+#    #+#             */
/*   Updated: 2025/05/27 20:42:58 by lvan-bre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.functions.h"

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
	len - Computes the maximum length between a string up to '=' and a
	given length.
	Used to align environment variable display formatting.
	Returns the greater of var's prefix length or the given length.
*/
static int	len(char *var, int strlen)
{
	int	varlen;

	varlen = ft_strlen_til_char(var, '=');
	if (varlen > strlen)
		return (varlen);
	return (strlen);
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
	while ((*envp)[i] && ft_strncmp((*envp)[i], str, len((*envp)[i], strlen)))
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
unsigned long	ft_export(char	***envp, char **cmd)
{
	unsigned long	res;
	unsigned long	tmp;
	int				i;

	res = 0;
	if (ft_darraylen(cmd) == 1)
		return (export_env(*envp));
	else
	{
		i = 1;
		while (cmd[i])
		{
			tmp = do_export(envp, cmd[i]);
			if (tmp != 0)
				res = tmp;
			i++;
		}
	}
	return (res);
}
