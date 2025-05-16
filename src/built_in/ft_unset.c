/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvan-bre <lvan-bre@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 16:51:30 by azhao             #+#    #+#             */
/*   Updated: 2025/05/16 00:34:40 by lvan-bre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.functions.h"

bool	unset_syntax(char *str)
{
	int	i;

	i = 0;
	if (!str[0] || (str[0] != '_' && !ft_isalpha(str[0])))
		return (false);
	while (str[i])
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (false);
		i++;
	}
	return (true);
}

int	existe(char *str, t_arg *env)
{
	int	i;
	int	len;

	if (!env)
		return (-1);
	i = 0;
	len = 0;
	while (str[len])
		len++;
	if (!strncmp(env->ctn, str, len))
		return (i);
	env = env->next;
	i++;
	while (env)
	{
		if (!strncmp(env->ctn, str, len))
			return (i);
		env = env->next;
		i++;
	}
	return (-1);
}

bool	unset_test(char *s, char **env)
{
	t_arg	*p;
	int		l;
	t_arg	*c;

	if (!s || !(*s))
		return (false);
	if (!unset_syntax(s))
		return (printerror("unset: invalid identifier\n"), true);
	p = NULL;
	c = *env;
	l = ft_strlen(s);
	while (env[j])
	{
		if (!ft_strncmp(c->ctn, s, l) && (c->ctn[l] == '=' || c->ctn[l] == '\0'))
		{
			if (p)
				p->next = c->next;
			else
				*env = c->next;
			return (free(c->ctn), free(c), false);
		}
		p = c;
		c = c->next;
	}
	return (false);
}

bool	unset_test(char *s, char **env)
{
	int		l;
	int		j;

	if (!s || !(*s))
		return (false);
	if (!unset_syntax(s))
		return (printerror("unset: invalid identifier\n"), true);
	j = 0;
	l = ft_strlen(s);
	while (env[j])
	{
		if (!ft_strncmp(env[j], s, l)
			&& (env[j][l] == '=' || env[j][l] == '\0'))
			return (free(env[j]), false);
		j++;
	}
	return (false);
}

int	ft_unset(char **env, char **str)
{
	int	exit_code;
	int	i;

	exit_code = 0;
	i = 0;
	while (str[i])
	{
		if (unset_test(str[i], env))
			exit_code = 1;
		i++;
	}
	return (exit_code);
}
