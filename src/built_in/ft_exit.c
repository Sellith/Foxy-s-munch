/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvan-bre <lvan-bre@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 19:18:01 by lvan-bre          #+#    #+#             */
/*   Updated: 2025/05/21 04:24:46 by lvan-bre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.functions.h"

/*
	This function is the executing of ft_exit, checking the exit conditions:
	if exit have no args it exits with the exit code of the former cmd
	if exit have an first arg that's not numeric it exits with an exit code of 2
	no matter how many args there are
	if exit have more than one arg with a first numeric arg it returns with an
	exit code of one without exiting
	if exit have one arg of more than 255 if overflows and start again at one.
 */
int	ft_exit(t_shell *data, char **cmd)
{
	char	*exitmsg;
	int		exitcode;

	exitcode = data->exitstatus;
	if (data->pipes == 0)
		exitmsg = ft_strdup("exit\n");
	else
		exitmsg = ft_strdup("");
	if (cmd[1] && !ft_isnum(cmd[1][0]))
	{
		ft_dprintf(STDERR_FILENO, "%s%s%s%s\n", exitmsg, EXIT_NUM_ERR1,
			cmd[1], EXIT_NUM_ERR2);
		so_long_exec(data, 2, exitmsg);
	}
	else if (ft_darraylen(cmd) > 2)
		return (ft_dprintf(STDERR_FILENO, "%s%s\n", exitmsg, EXIT_ARGS_ERR),
			free(exitmsg), 1);
	else
	{
		ft_dprintf(STDERR_FILENO, "%s", exitmsg);
		if (cmd[1])
			exitcode = ft_atoi(cmd[1]);
		so_long_exec(data, exitcode % 256, exitmsg);
	}
	return (0);
}
