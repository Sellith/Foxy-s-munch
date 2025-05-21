/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvan-bre <lvan-bre@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 17:02:57 by azhao             #+#    #+#             */
/*   Updated: 2025/05/21 04:16:23 by lvan-bre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.functions.h"

/*
	This function is the executing of ft_env it skips the $? since it is a local
	variable.
	Should there be any option, argument or any write error it will exit with
	an exit code accordingly to the error.
 */
int	ft_env(char **cmd, char **env)
{
	int		i;

	i = 0;
	if (!env && !(*env)[0])
		return (0);
	if (cmd[1] && cmd[1][0] == '-')
		return (ft_printf("%e", ENV_OPT_ERR), 125);
	else if (ft_darraylen(cmd) > 1)
		return (ft_printf("%e", ENV_ARG_ERR), 127);
	while (env[i])
	{
		if (ft_strncmp(env[i], "?=", 2) == 0)
			i++;
		if (!env[i])
			return (0);
		if (write(STDOUT_FILENO, env[i], ft_strlen(env[i])) == -1)
			return (perror(ENV_WRITE_ERR), 1);
		if ((write(STDOUT_FILENO, "\n", 1)) == -1)
			return (perror(ENV_WRITE_ERR), 1);
		i++;
	}
	return (0);
}
