/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvan-bre <lvan-bre@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 22:18:30 by lvan-bre          #+#    #+#             */
/*   Updated: 2025/05/15 21:24:35 by lvan-bre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.functions.h"

static char	*expend_env(char *raw, char *cooked, char **env, int *x)
{
	char	*buffer;
	char	*tmp;
	int		len;
	int		i;

	len = 0;
	while (is_envable(raw[len]))
		len++;
	if (len == 0)
		return (ft_strjoining(cooked, "$"));
	*x += len;
	buffer = ft_strndup(raw, len);
	if (!buffer)
		return (NULL);
	i = 0;
	while (env[i] && ft_strncmpcat(buffer, env[i], "=", len + 1) != 0)
		i++;
	free(buffer);
	if (!env[i])
		tmp = ft_strdup("");
	else
		tmp = ft_strdup(env[i] + len + 1);
	buffer = ft_strjoining(cooked, tmp);
	return (free(tmp), buffer);
}

static char	*dup_til_env(t_arg *arg, char *raw, char *cooked, int *x)
{
	char	*tmp;
	int		len;

	if (!arg->q_trace)
		len = ft_strlen_til_char(raw, '$');
	else
		len = ft_strlen(raw);
	*x += len;
	tmp = ft_strndup(raw, len);
	if (!tmp)
		return (NULL);
	cooked = ft_strjoining(cooked, tmp);
	free(tmp);
	return (cooked);
}

char	*add_to_hd(t_arg *arg, char *line, char *sent, char **env)
{
	char	*buffer;
	int		i;

	i = 0;
	buffer = NULL;
	while (sent[i])
	{
		if (!arg->q_trace && sent[i] == '$')
		{
			i++;
			buffer = expend_env(sent + i, buffer, env, &i);
		}
		else
			buffer = dup_til_env(arg, sent + i, buffer, &i);
		if (!buffer)
			return (NULL);
	}
	if (!buffer)
		return (ft_strjoining(line, " "));
	line = ft_strjoining(line, buffer);
	if (!line)
		return (NULL);
	return (free(buffer), line);
}
