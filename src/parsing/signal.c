/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvan-bre <lvan-bre@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 20:52:40 by lvan-bre          #+#    #+#             */
/*   Updated: 2025/05/15 21:24:33 by lvan-bre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.functions.h"

int	g_sig = 0;

void	sigint_nl(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	g_sig = 130;
}

static void	sigint_hd(int sig)
{
	(void)sig;
	close(STDIN_FILENO);
	ft_printf("\n");
	g_sig = 130;
}

void	init_signals(t_setsig set)
{
	if (set == S_INTERACTIVE)
	{
		sigaction(SIGINT, &(struct sigaction){.sa_handler = sigint_nl}, NULL);
		signal(SIGQUIT, SIG_IGN);
		signal(SIGPIPE, SIG_IGN);
	}
	else if (set == S_HEREDOC)
	{
		sigaction(SIGINT, &(struct sigaction){.sa_handler = sigint_hd}, NULL);
		signal(SIGQUIT, SIG_IGN);
		signal(SIGPIPE, SIG_IGN);
	}
	else if (set == S_IGNORE)
	{
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
		signal(SIGPIPE, SIG_IGN);
	}
	else if (set == S_DEFAULT)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		signal(SIGPIPE, SIG_DFL);
	}
}
