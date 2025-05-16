/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_functions.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvan-bre <lvan-bre@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 00:50:27 by lvan-bre          #+#    #+#             */
/*   Updated: 2025/05/15 21:24:33 by lvan-bre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.functions.h"

static void	print_arg(t_arg *arg, char *type, int i)
{
	t_arg	*head;
	int		j;

	head = arg;
	j = 0;
	while (head)
	{
		ft_printf("%i's pipe %i's %s is : \n%s\n", i, j, type, head->ctn);
		head = head->next;
		j++;
	}
}

void	print_lists(t_mlst *lst)
{
	t_mlst	*head;
	int		i;

	i = 0;
	head = lst;
	while (head)
	{
		if (head->cmd)
			print_arg(head->cmd, "cmd arg", i);
		if (head->eof)
		{
			print_arg(head->eof, "eof", i);
			ft_printf("Hd ctn :\n%s", lst->hd);
		}
		if (head->inf)
			print_arg(head->inf, "inf", i);
		if (head->trunc)
			print_arg(head->trunc, "trunc", i);
		if (head->append)
			print_arg(head->append, "append", i);
		ft_printf("last infile type is : %i\n", head->rdir[INF]);
		ft_printf("last outfile type is : %i\n", head->rdir[OUT]);
		head = head->next;
		i++;
	}
}
