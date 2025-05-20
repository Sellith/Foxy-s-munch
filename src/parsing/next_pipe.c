/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   next_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sellith <sellith@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 03:52:34 by lvan-bre          #+#    #+#             */
/*   Updated: 2025/05/19 18:37:52 by sellith          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.functions.h"

static int	child_rl(t_shell *data, int w_fd)
{
	char	*buffer;

	init_signals(S_HEREDOC);
	buffer = readline("> ");
	while (buffer && !*buffer && g_sig != 130)
	{
		free(buffer);
		buffer = readline("> ");
	}
	freerpipe(data);
	if (g_sig == 130)
		return (close(w_fd), 130);
	write(w_fd, buffer, ft_strlen(buffer));
	free(buffer);
	close(w_fd);
	return (0);
}

static bool	pipe_rl(t_shell *data)
{
	char	*buffer;
	int		status;
	int		fd[2];
	int		pid;

	if (pipe(fd) == -1)
		return (ft_printf("%s\n", PIPE_ERR), false);
	pid = fork();
	if (pid == -1)
		return (ft_printf("%s\n", FORK_ERR), false);
	closefds(fd, pid);
	if (pid == 0)
		exit(child_rl(data, fd[WRITE]));
	buffer = get_n_close(fd[READ]);
	wait(&status);
	data->exitstatus = WEXITSTATUS(status);
	if (data->exitstatus != 0)
		return (false);
	if (!buffer)
		so_long_minishell(data, 2);
	data->line = ft_strjoining(data->line, buffer);
	free(buffer);
	if (!data->line)
		return (false);
	return (true);
}

bool	next_pipe(t_shell *dt)
{
	ft_str_replace(&dt->line, dt->line + 1);
	if (*dt->line == '|')
	{
		dt->exitstatus = 1;
		return (ft_printf("%s", PARS_ERR), false);
	}
	dt->pipes++;
	dt->line = ft_rmchars(dt->line, ' ');
	dt->line = ft_rmchars(dt->line, '\t');
	if (*dt->line == '\n' || !*dt->line)
	{
		if (!pipe_rl(dt))
			return (false);
	}
	return (true);
}
