/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvan-bre <lvan-bre@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 22:38:23 by lvan-bre          #+#    #+#             */
/*   Updated: 2025/05/16 05:54:33 by lvan-bre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.functions.h"

static char	*usr_to_tilde(char *pwd)
{
	char	*buffer;
	int		skiplen;
	int		cwdlen;
	int		skip;

	cwdlen = ft_strlen(pwd);
	skip = 6;
	if (ft_strncmp(pwd, "PWD=", 4) == 0)
		skip = 10;
	if (cwdlen <= skip)
		return (ft_strdup(pwd));
	skiplen = ft_strlen_til_char(pwd + skip, '/') + skip;
	buffer = ft_strjoin("~", pwd + skiplen);
	return (buffer);
}

static char	*make_prompt(char *pwd)
{
	char	*buffer;
	char	*tmp;

	buffer = ft_strjoin("⊢", WTH);
	buffer = ft_strdjoining(buffer, "[", PPL);
	buffer = ft_strdjoining(buffer, "Foxy's Munch", WTH);
	buffer = ft_strdjoining(buffer, "]-[", GRN);
	tmp = usr_to_tilde(pwd);
	if (!tmp)
		return (NULL);
	buffer = ft_strdjoining(buffer, tmp, WTH);
	free(tmp);
	buffer = ft_strdjoining(buffer, "] > ", RESET);
	return (buffer);
}

static void	prompt_rl(t_shell *data)
{
	data->prompt = make_prompt(data->pwd);
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

int	main(int argc, char **argv)
{
	t_shell	data;
	char **envp;

	(void)argv;
	if (argc != 1)
		return (ft_printf("%s", ERR_ARGS), 0);
	envp = NULL;
	initdata(&data);
	if (envp)
	{
		data.envp = ft_arraydup(envp);
		if (!data.envp)
			return (1);
	}
	init_pwd(&data);
	data.exitstatus = 0;
	shell_lvl(&data);
	last_cmd_status(&data);
	data.path = get_path(data.envp);
	if (!data.path)
		return (ft_freeall("%d%S", &data.envp, &data.ut), 1);
	while (1)
		prompt_rl(&data);
	return (0);
}
