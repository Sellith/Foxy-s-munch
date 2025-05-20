/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sellith <sellith@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 16:51:30 by azhao             #+#    #+#             */
/*   Updated: 2025/05/20 03:24:12 by sellith          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.functions.h"

// bool	unset_syntax(char *str)
// {
// 	int	i;

// 	i = 0;
// 	if (!str[0] || (str[0] != '_' && !ft_isalpha(str[0])))
// 		return (false);
// 	while (str[i])
// 	{
// 		if (!ft_isalnum(str[i]) && str[i] != '_')
// 			return (false);
// 		i++;
// 	}
// 	return (true);
// }

// int	existe(char *str, t_arg *env)
// {
// 	int	i;
// 	int	len;

// 	if (!env)
// 		return (-1);
// 	i = 0;
// 	len = 0;
// 	while (str[len])
// 		len++;
// 	if (!strncmp(env->ctn, str, len))
// 		return (i);
// 	env = env->next;
// 	i++;
// 	while (env)
// 	{
// 		if (!strncmp(env->ctn, str, len))
// 			return (i);
// 		env = env->next;
// 		i++;
// 	}
// 	return (-1);
// }

// bool	unset_test(char *s, char **env)
// {
// 	t_arg	*p;
// 	int		l;
// 	t_arg	*c;

// 	if (!s || !(*s))
// 		return (false);
// 	if (!unset_syntax(s))
// 		return (printerror("unset: invalid identifier\n"), true);
// 	p = NULL;
// 	c = *env;
// 	l = ft_strlen(s);
// 	while (env[j])
// 	{
// 		if (!ft_strncmp(c->ctn, s, l) && (c->ctn[l] == '=' || c->ctn[l] == '\0'))
// 		{
// 			if (p)
// 				p->next = c->next;
// 			else
// 				*env = c->next;
// 			return (free(c->ctn), free(c), false);
// 		}
// 		p = c;
// 		c = c->next;
// 	}
// 	return (false);
// }

// bool	unset_test(char *s, char **env)
// {
// 	int		l;
// 	int		j;

// 	if (!s || !(*s))
// 		return (false);
// 	if (!unset_syntax(s))
// 		return (printerror("unset: invalid identifier\n"), true);
// 	j = 0;
// 	l = ft_strlen(s);
// 	while (env[j])
// 	{
// 		if (!ft_strncmp(env[j], s, l)
// 			&& (env[j][l] == '=' || env[j][l] == '\0'))
// 			return (free(env[j]), false);
// 		j++;
// 	}
// 	return (false);
// }

// int	ft_unset(char **env, char **str)
// {
// 	int	exit_code;
// 	int	i;

// 	exit_code = 0;
// 	i = 0;
// 	while (str[i])
// 	{
// 		if (unset_test(str[i], env))
// 			exit_code = 1;
// 		i++;
// 	}
// 	return (exit_code);
// }

static int	count_args(t_shell *data, char **cmd)
{
	int	res;
	int	i;

	res = 0;
	i = 0;
	while (cmd[i])
	{
		if (search_env_var(data->envp, cmd[i]) != -1)
			res++;
		i++;
	}
	return (res);
}

static bool	check_env(char **cmd, char *var, int args)
{
	int	i;

	i = 0;
	while (i < args)
	{
		if (ft_strncmpcat(cmd[i], var, "=", ft_strlen(cmd[i]) + 1) == 0)
			return (true);
		i++;
	}
	return (false);
}

static char	**do_unset(t_shell *data, char **cmd)
{
	char	**buffer;
	int		envplen;
	int		args;
	
	args = count_args(data, cmd);
	envplen = ft_darraylen(data->envp);
	buffer = ft_calloc(envplen - args + 1, sizeof(char *));
	if (!buffer)
		return (NULL);
	while (data->ut->i < envplen - args)
	{
		if (check_env(cmd, data->envp[data->ut->j], args))
			data->ut->j++;
		buffer[data->ut->i] = ft_strdup(data->envp[data->ut->j++]);
		if (!buffer[data->ut->i])
			return (ft_freeall("%d", &buffer), NULL);
		data->ut->i++;
	}
	buffer[data->ut->i] = NULL;
	ft_freeall("%d", &data->envp);
	return (buffer);
}

int	ft_unset(char **cmd, t_shell *data)
{
	if (!cmd[1])
		return (0);
	if ((cmd[1][0] == '-'))
		return (ft_printf("%e", UNSET_OPT_ERR), 2);
	reinit_ut(data->ut);
	data->envp = do_unset(data, cmd + 1);
	if (!data->envp)
		return (2);
	return (0);
}
