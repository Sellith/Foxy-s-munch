/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvan-bre <lvan-bre@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 05:50:17 by lvan-bre          #+#    #+#             */
/*   Updated: 2025/05/21 22:00:54 by lvan-bre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.functions.h"

/*
	Removes the first dt->lsize characters from line.
	Returns a new allocated string with the remaining content.
	Frees the old line to avoid memory leaks.
*/
char	*rm_from_line(t_shell *dt, char *line)
{
	char	*buffer;

	buffer = ft_strdup(line + dt->lsize);
	if (line)
		free(line);
	return (buffer);
}

/*
	Handles the heredoc setup for the command:
	Calls select_tolkien to parse heredoc delimiters.
	If successful, calls heredoc to process the heredoc content.
	Frees the command structure and returns false on failure.
	Returns true on success, false otherwise.
*/
bool	handle_hd(t_shell *dt, t_mlst *new)
{
	if (selec_tolkien(dt, new, DOUB, INF))
	{
		new->hd = heredoc(dt, new, new->eof);
		if (!new->hd)
			return (freemlst(new), false);
		return (true);
	}
	return (false);
}

/*
	Parses the input line and populates the command list new:
	- Removes spaces and tabs at the start of dt->line.
	- Processes redirections (heredoc <<, append >>, input <, output >).
	- Adds commands or arguments otherwise.
	- Stops parsing on pipe |, newline \n, or end of line.
	Returns true if all parsing succeeds, false on any error.
*/
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
			ret = selec_tolkien(dt, new, DOUB, OUT);
		else if (*dt->line == '<')
			ret = selec_tolkien(dt, new, SING, INF);
		else if (*dt->line == '>')
			ret = selec_tolkien(dt, new, SING, OUT);
		else
			ret = selec_tolkien(dt, new, NUL, CMD);
		if (ret == false)
			return (false);
	}
	return (true);
}

/*
	Adds a new node new at the end of the doubly linked list mlst.
	If the list is empty, initializes it with new.
	Otherwise, traverses to the last node and appends new, updating
	prev and next pointers accordingly.
*/
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

/*
	Parses the input line dt->line into a linked list of command structures.
	- Allocates a new list node for each command segment.
	- Removes spaces and tabs from the current line start.
	- Checks for syntax errors such as unexpected pipe at start.
	- Initializes and fills the list node with command and redirection info.
	- Adds the node to the master list.
	- Advances past pipe symbols to parse next command segment.
	Returns true on success, false on any allocation or parsing error.
*/
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
		add_bach(&dt->mlst, new);
		if (*(dt->line) == '|')
			if (!next_pipe(dt))
				return (false);
	}
	return (true);
}
