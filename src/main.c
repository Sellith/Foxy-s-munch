/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sellith <sellith@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 22:38:23 by lvan-bre          #+#    #+#             */
/*   Updated: 2025/05/20 03:32:33 by sellith          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.functions.h"

static char	*usr_to_tilde(t_shell *data, char *pwd)
{
	char	*buffer;
	int		skiplen;
	int		cwdlen;
	int		homelen;

	cwdlen = ft_strlen(pwd);
	if (data->home)
	{
		homelen = ft_strlen(data->home);
		if (data->home[homelen - 1] == '/')
			homelen--;
		if (cwdlen < homelen)
			return (ft_strdup(pwd));
		buffer = ft_strjoin("~", pwd + homelen);
	}
	else
	{
		homelen = 6;
		if (cwdlen <= homelen)
			return (ft_strdup(pwd));
		skiplen = ft_strlen_til_char(pwd + homelen, '/') + homelen;
		buffer = ft_strjoin("~", pwd + skiplen);
	}
	return (buffer);
}

static char	*make_prompt(t_shell *data)
{
	char	*buffer;
	char	*tmp;

	buffer = ft_strjoin("⊢", WTH);
	buffer = ft_strdjoining(buffer, "[", PPL);
	buffer = ft_strdjoining(buffer, "Foxy's Munch", WTH);
	buffer = ft_strdjoining(buffer, "]-[", GRN);
	tmp = usr_to_tilde(data, data->pwd);
	if (!tmp)
		return (NULL);
	buffer = ft_strdjoining(buffer, tmp, WTH);
	free(tmp);
	buffer = ft_strdjoining(buffer, "] > ", RESET);
	return (buffer);
}

static void	prompt_rl(t_shell *data)
{
	data->prompt = make_prompt(data);
	if (!data->prompt)
		return ;
	init_signals(S_INTERACTIVE);
	data->line = readline(data->prompt);
	init_signals(S_IGNORE);
	if (!data->line)
		return (so_long_minishell(data, 0));
	if (ft_strlen(data->line) != 0)
		add_history(data->line);
	if (g_sig != 0)
		exit_sig(data);
	if (ft_strlen(data->line) != 0 && parsing(data))
	{
		data->exitstatus = 0;
		minishell(data, data->mlst);
		last_cmd_status(data);
	}
	reset_data(data, &data->mlst);
}

static void	shell_lvl(t_shell *data)
{
	char	*buffer;
	char	*tmp;
	int		shlvl;
	int		i;

	i = 0;
	while (data->envp && data->envp[i]
		&& ft_strncmp(data->envp[i], "SHLVL=", 2) != 0)
		i++;
	if (data->envp[i])
	{
		shlvl = ft_atoi(data->envp[i] + 6) + 1;
		tmp = ft_itoa(shlvl);
	}
	else
		tmp = ft_strdup("1");
	buffer = ft_strjoin("SHLVL=", tmp);
	if (!data->envp[i])
		data->envp = ft_addtoda(data->envp, buffer);
	else
	{
		free(data->envp[i]);
		data->envp[i] = ft_strdup(buffer);
	}
	(free(buffer), free(tmp));
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	data;

	(void)argv;
	if (argc != 1)
		return (ft_printf("%s", ERR_ARGS), 0);
	initdata(&data);
	if (envp)
	{
		data.envp = ft_arraydup(envp);
		if (!data.envp)
			return (1);
	}
	init_pwd(&data);
	shell_lvl(&data);
	last_cmd_status(&data);
	while (1)
	{
		data.path = get_path(&data, data.envp);
		if (!data.path)
			return (ft_freeall("%d%S", &data.envp, &data.ut), 1);
		get_home(&data, data.envp);
		prompt_rl(&data);
	}
	return (0);
}
