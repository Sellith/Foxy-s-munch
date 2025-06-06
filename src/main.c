/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvan-bre <lvan-bre@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 22:38:23 by lvan-bre          #+#    #+#             */
/*   Updated: 2025/05/26 23:51:01 by lvan-bre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.functions.h"

/*
	Replaces the home directory path in pwd with '~' if applicable.
	If pwd starts with data->home, it returns a new string replacing that
	prefix by '~'.
	Otherwise, returns a copy of pwd
	Returns allocated string or NULL on failure.
 */
static char	*usr_to_tilde(t_shell *data, char *pwd)
{
	char	*buffer;
	int		cwdlen;
	int		homelen;
	int		var;

	cwdlen = ft_strlen(pwd);
	var = search_env_var(data->envp, "HOME=");
	if (var != -1 && ft_strlen(data->envp[var]) > 5)
	{
		homelen = ft_strlen(data->home);
		if (data->home[homelen - 1] == '/')
			homelen--;
		if (cwdlen < homelen)
			return (ft_strdup(pwd));
		buffer = ft_strjoin("~", pwd + homelen);
	}
	else
		buffer = ft_strdup(pwd);
	return (buffer);
}

/*
	Creates and returns the shell prompt string with colors and username.
	It formats the prompt as "⊢[Foxy's Munch]-[repertory] > ".
	Returns the allocated prompt string or NULL on failure.
 */
static char	*make_prompt(t_shell *data)
{
	char	*buffer;
	char	*tmp;

	buffer = ft_strjoin("⊢", WTH);
	buffer = ft_strdjoining(buffer, "[", PPL);
	buffer = ft_strdjoining(buffer, data->usr, WTH);
	buffer = ft_strdjoining(buffer, "]-[", GRN);
	tmp = usr_to_tilde(data, data->pwd);
	if (!tmp)
		return (NULL);
	buffer = ft_strdjoining(buffer, tmp, WTH);
	free(tmp);
	buffer = ft_strdjoining(buffer, "] > ", RESET);
	return (buffer);
}

/*
	This function handles the interactive shell prompt using readline.
	It initializes signals, reads user input, adds non-empty lines to history,
	and executes commands if parsing succeeds.
	Exits or resets state appropriately based on input and signals.
 */
static void	prompt_rl(t_shell *data)
{
	data->path = get_path(data, data->envp);
	data->prompt = make_prompt(data);
	if (!data->prompt)
		return ;
	init_signals(S_INTERACTIVE);
	data->line = readline(data->prompt);
	init_signals(S_IGNORE);
	if (g_sig != 0)
		exit_sig(data);
	if (!data->line)
		return (so_long_minishell(data, data->exitstatus));
	if (ft_strlen(data->line) != 0)
		add_history(data->line);
	if (ft_strlen(data->line) != 0 && parsing(data))
	{
		data->stdin_clone = dup(STDIN_FILENO);
		minishell(data, data->mlst);
	}
	last_cmd_status(data);
	reset_data(data, &data->mlst);
}

/*
	This function increments the SHLVL environment variable by 1.
	If SHLVL is not set, it initializes it to 1.
	It updates or adds the SHLVL variable in the shell environment.
 */
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

/*
	Main function initializing shell data and environment,
	checking arguments, and entering the main prompt loop.
	Handles environment duplication, PWD and SHLVL setup,
	and cleans up on errors or exit.
 */
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
	data.usr = get_user();
	init_pwd(&data);
	shell_lvl(&data);
	last_cmd_status(&data);
	while (1)
	{
		get_home(&data, data.envp);
		prompt_rl(&data);
	}
	return (0);
}
