/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvan-bre <lvan-bre@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 05:50:17 by lvan-bre          #+#    #+#             */
/*   Updated: 2025/05/15 21:24:33 by lvan-bre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.functions.h"

char	*rm_from_line(t_shell *dt, char *line)
{
	char	*buffer;

	buffer = ft_strdup(line + dt->lsize);
	if (line)
		free(line);
	return (buffer);
}

bool	handle_hd(t_shell *dt, t_mlst *new)
{
	if (select_tolkien(dt, new, DOUB, INF))
	{
		new->hd = heredoc(dt, new, new->eof);
		if (!new->hd)
			return (freemlst(new), false);
		return (true);
	}
	return (false);
}

static bool	put_to_list(t_shell *dt, t_mlst *new)
{
	bool	ret;

	while (dt->line && *dt->line != '\n' && *dt->line && (*dt->line != '|'))
	{
		dt->line = ft_rmchars(dt->line, ' ');
		dt->line = ft_rmchars(dt->line, '\t');
		if (!*dt->line || *dt->line == '|' || *dt->line == '\n')
			break ;
		if (dt->line[0] == '<' && dt->line[1] == '<')
			ret = handle_hd(dt, new);
		else if (dt->line[0] == '>' && dt->line[1] == '>')
			ret = select_tolkien(dt, new, DOUB, OUT);
		else if (*dt->line == '<')
			ret = select_tolkien(dt, new, SING, INF);
		else if (*dt->line == '>')
			ret = select_tolkien(dt, new, SING, OUT);
		else
			ret = select_tolkien(dt, new, NUL, CMD);
		if (ret == false)
			return (false);
	}
	return (true);
}

static void	add_bach(t_mlst **mlst, t_mlst *new)
{
	t_mlst	*head;

	if (!*mlst)
	{
		*mlst = new;
		(*mlst)->prev = NULL;
		return ;
	}
	head = *mlst;
	while (head->next)
		head = head->next;
	head->next = new;
	new->prev = head;
}

bool	parsing(t_shell *dt)
{
	t_mlst	*new;

	while (dt->line && *dt->line != '\n' && *dt->line)
	{
		new = ft_calloc(1, sizeof(t_mlst));
		if (!new)
			return (false);
		dt->line = ft_rmchars(dt->line, ' ');
		dt->line = ft_rmchars(dt->line, '\t');
		if (*dt->line == '|')
		{
			dt->exitstatus = 1;
			return (free(new), ft_printf("%s", PARS_ERR), false);
		}
		if (!initmlst(&new) || !put_to_list(dt, new))
			return (false);
		new->next = NULL;
		add_bach(&dt->mlst, new);
		if (*(dt->line) == '|')
			if (!next_pipe(dt))
				return (false);
	}
	return (true);
}
