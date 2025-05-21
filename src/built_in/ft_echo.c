/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvan-bre <lvan-bre@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 15:30:23 by azhao             #+#    #+#             */
/*   Updated: 2025/05/21 04:12:41 by lvan-bre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.functions.h"

/*
	This function check if the '-n' option is triggered to check if there will
	be a newline.
 */
bool	check_newline(char *str)
{
	unsigned int	i;

	i = 0;
	if (str && str[i] && str[i] == '-')
	{
		i++;
		if (!str[i])
			return (true);
		while (str[i] && str[i] == 'n')
			i++;
		if (i == ft_strlen(str))
			return (false);
	}
	return (true);
}

/*
	This function is the executing of echo, writing echo and checking for
	errors, and puting a newline if the newline function returns true.

	Should there be a write error the function will return 1.
 */
int	ft_echo(char **cmd)
{
	bool	newline;
	int		count;
	int		i;

	i = 1;
	count = ft_darraylen(cmd);
	newline = check_newline(cmd[i]);
	if (!newline)
		i++;
	while (i < count)
	{
		if (write(1, cmd[i], ft_strlen(cmd[i])) == -1)
			return (perror(ECHO_WRITE_ERR), 1);
		if (i != count - 1)
			if (write(1, " ", 1) == -1)
				return (perror(ECHO_WRITE_ERR), 1);
		++i;
	}
	if (newline == true)
		if (write(1, "\n", 1) == -1)
			return (perror(ECHO_WRITE_ERR), 1);
	return (0);
}
