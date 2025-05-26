/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_env.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvan-bre <lvan-bre@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 23:59:51 by lvan-bre          #+#    #+#             */
/*   Updated: 2025/05/27 00:12:58 by lvan-bre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.functions.h"

/*
	This function prints the environment in the format of the export command.
	It skips the local variable "?=" since it's a local variable and handles
	write errors.
	Returns 1 on write error, 0 otherwise.
 */
unsigned long	export_env(char **env)
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
