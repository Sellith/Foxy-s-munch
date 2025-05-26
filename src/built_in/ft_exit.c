/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvan-bre <lvan-bre@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 19:18:01 by lvan-bre          #+#    #+#             */
/*   Updated: 2025/05/26 23:57:29 by lvan-bre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.functions.h"

/*
	ft_check_ulong_str - Validates if a string can be safely cast to an
	unsigned long.
	Rejects non-numeric, overly long, or out-of-range values based on 64-bit
	limits.
	Handles optional '+' or '-' signs and compares against max unsigned
	limits allowing an overflow.
	Returns false on any invalid case, true otherwise.
*/
static bool	ft_check_ulong_str(char *str)
{
	int	i;

	if (!str || !ft_isatoiable(str[0]))
		return (false);
	i = 1;
	while (str[i])
		if (!ft_isnum(str[i++]))
			return (false);
	if (!ft_isnum(str[0]) && i > 21)
		return (false);
	if (str[0] == '+' && i <= 21
		&& ft_strncmp("9223372036854775807", str + 1, 20) < 0)
		return (false);
	if (str[0] == '-' && i <= 21
		&& ft_strncmp("9223372036854775808", str + 1, 20) < 0)
		return (false);
	if (ft_isnum(str[0]) && i > 20)
		return (false);
	if (ft_isnum(str[0]) && i <= 20
		&& ft_strncmp("9223372036854775807", str, 20) < 0)
		return (false);
	return (true);
}

/*
	no_exit - Handles the case where 'exit' is called with too many arguments.
	Prints an error message and resets the message string.
	Returns 1 if exit code is 0, otherwise returns the given exit code.
*/
static int	no_exit(char **msg, unsigned long exitc)
{
	ft_dprintf(STDERR_FILENO, "%s%s\n", *msg, EXIT_ARGS_ERR);
	ft_str_reset(msg);
	if (exitc == 0)
		return (1);
	return (exitc);
}

/*
	This function is the executing of ft_exit, checking the exit conditions:
	if exit have no args it exits with the exit code of the former cmd
	if exit have an first arg that's not numeric it exits with an exit code of 2
	no matter how many args there are
	if exit have more than one arg with a first numeric arg it returns with an
	exit code of one without exiting
	if exit have one arg of more than 255 if overflows and start again at one.
 */
unsigned long	ft_exit(t_shell *data, char **cmd)
{
	char			*msg;
	unsigned long	exitc;

	exitc = data->exitstatus;
	if (data->pipes == 0)
		msg = ft_strdup("exit\n");
	else
		msg = ft_strdup("");
	if (cmd[1] && (!ft_check_ulong_str(cmd[1])))
	{
		ft_dprintf(2, "%s%s%s%s\n", msg, EXIT_ERR, cmd[1], EXIT_NUM_ERR);
		ft_darray_reset(&cmd);
		so_long_exec(data, 2, msg);
	}
	else if (ft_darraylen(cmd) > 2)
		return (no_exit(&msg, exitc));
	else
	{
		ft_dprintf(STDERR_FILENO, "%s", msg);
		if (cmd[1])
			exitc = ft_atoul(cmd[1]);
		ft_darray_reset(&cmd);
		so_long_exec(data, exitc % 256, msg);
	}
	return (0);
}
