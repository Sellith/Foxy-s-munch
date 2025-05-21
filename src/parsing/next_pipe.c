/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   next_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvan-bre <lvan-bre@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 03:52:34 by lvan-bre          #+#    #+#             */
/*   Updated: 2025/05/21 05:37:27 by lvan-bre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.functions.h"

/*
	Child process for reading a continuation line after a trailing pipe.
	It installs heredoc-specific signal handlers, reads a line from stdin,
	skips empty lines, handles SIGINT (g_sig == 130) by closing the pipe and
	exiting, then writes the line to the pipe and closes the write end before
	exiting with status 0.
*/
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

/*
	Handles reading additional input for a trailing pipe in the command line.
	Creates a pipe and forks a child process to read the continuation line.
	The parent waits for the child and appends the new input to data->line.
	Returns true on success, false on error or if the child exits with
	non-zero status.
*/
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

/*
	Moves the parsing position past the current pipe character.
	Checks for syntax errors if another pipe follows immediately.
	Increments the pipe count.
	Removes leading spaces and tabs after the pipe.
	If the line ends or has a newline, triggers pipe read-line handling.
	Returns true if parsing can continue, false on error.
*/
bool	next_pipe(t_shell *dt)
{
	ft_str_replace(&dt->line, dt->line + 1);
	if (!dt->line)
		return (false);
	if (*dt->line == '|')
	{
		dt->exitstatus = 1;
		return (ft_printf("%s", PARS_ERR), false);
	}
	dt->pipes++;
	dt->line = ft_rmchars(dt->line, ' ');
	if (!dt->line)
		return (false);
	dt->line = ft_rmchars(dt->line, '\t');
	if (!dt->line)
		return (false);
	if (*dt->line == '\n' || !*dt->line)
	{
		if (!pipe_rl(dt))
			return (false);
	}
	return (true);
}
