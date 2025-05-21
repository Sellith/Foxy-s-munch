/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvan-bre <lvan-bre@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 15:30:23 by azhao             #+#    #+#             */
/*   Updated: 2025/05/21 02:52:21 by lvan-bre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.functions.h"

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

static int	write_echo(char **cmd, bool newline, int i, int count)
{
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

int	ft_echo(char **cmd)
{
	write_echo(cmd, true, 1, ft_darraylen(cmd));
	return (0);
}
