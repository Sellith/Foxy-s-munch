/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   put_ctn.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvan-bre <lvan-bre@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 20:28:25 by lvan-bre          #+#    #+#             */
/*   Updated: 2025/05/15 21:24:33 by lvan-bre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.functions.h"

static char	*ft_putenv(char *cooked, char *raw, t_shell *dt, t_arg *ag)
{
	char	*buffer;
	int		len;
	int		i;

	len = 0;
	while (is_envable(raw[len + 1]))
		len++;
	if (len == 0 && ((raw[1] != '\"' && raw[1] != '\'') || ag->q))
		return (ft_strjoining(cooked, "$"));
	else if (len == 0 && (raw[1] == '\"' || raw[1] == '\''))
		return (ft_strfdup(cooked));
	dt->ut->i += len;
	i = 0;
	buffer = ft_strndup(raw + 1, len);
	while (dt->envp[i] && ft_strncmpcat(buffer, dt->envp[i], "=", len + 1) != 0)
		i++;
	free(buffer);
	if (!dt->envp[i] && cooked)
		buffer = ft_strfdup(cooked);
	else if (!dt->envp[i] && !cooked)
		buffer = ft_strdup("");
	else
		buffer = ft_strjoining(cooked, dt->envp[i] + (len + 1));
	return (buffer);
}

char	*char_til_env(char *cooked, char *raw, t_utils **ut, t_arg *ag)
{
	char	*buffer;
	int		size;

	(*ut)->len = 0;
	while (raw[(*ut)->len] && ((ag->q && raw[(*ut)->len] != (*ut)->c)
			|| (!ag->q && raw[(*ut)->len] != '\''
				&& raw[(*ut)->len] != '\"')))
	{
		if (raw[(*ut)->len] == '$' && ((ag->q && (*ut)->c == '\"') || !ag->q))
			break ;
		(*ut)->len++;
	}
	(*ut)->i += (*ut)->len - 1;
	size = ft_strlen(cooked);
	buffer = ft_calloc((*ut)->len + size + 1, sizeof(char));
	if (!buffer)
		return (NULL);
	ft_strcpy(buffer, cooked);
	if (cooked)
		free(cooked);
	(*ut)->j = -1;
	while (++(*ut)->j < (*ut)->len)
		buffer[size + (*ut)->j] = raw[(*ut)->j];
	buffer[size + (*ut)->j] = '\0';
	return (buffer);
}

void	distribution(t_arg *ag, t_shell *dt, char **cooked, char *raw)
{
	if (((ag->q && dt->ut->c == '\"') || !ag->q) && raw[dt->ut->i] == '$')
		*cooked = ft_putenv(*cooked, raw + dt->ut->i, dt, ag);
	else
		*cooked = char_til_env(*cooked, raw + dt->ut->i, &dt->ut, ag);
}

char	*put_ctn(char *arg, t_utils *ut, t_arg *ag, t_shell *dt)
{
	char	*buffer;

	ag->q = false;
	buffer = NULL;
	ut->i = 0;
	while (arg && arg[ut->i])
	{
		if (!ag->q && (arg[ut->i] == '\'' || arg[ut->i] == '\"'))
		{
			ag->q = true;
			ut->c = arg[ut->i++];
		}
		if (ag->q && arg[ut->i] == ut->c)
		{
			ag->q = false;
			ut->i++;
		}
		distribution(ag, dt, &buffer, arg);
		ut->i++;
	}
	return (buffer);
}

void	arg_bark(t_arg **cmd, t_arg *new, t_shell *dt)
{
	t_arg	*head;

	dt->line = rm_from_line(dt, dt->line);
	if (!*cmd)
	{
		*cmd = new;
		return ;
	}
	head = *cmd;
	while (head->next)
		head = head->next;
	head->next = new;
}
