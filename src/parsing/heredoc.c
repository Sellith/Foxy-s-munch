/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvan-bre <lvan-bre@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 03:17:08 by lvan-bre          #+#    #+#             */
/*   Updated: 2025/05/15 21:24:33 by lvan-bre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.functions.h"

static char	*char_til_q(char *cooked, char *raw, t_utils **ut, t_arg *ag)
{
	char	*buffer;
	int		size;

	(*ut)->len = 0;
	while (raw[(*ut)->len] && ((ag->q && raw[(*ut)->len] != (*ut)->c)
			|| (!ag->q && raw[(*ut)->len] != '\''
				&& raw[(*ut)->len] != '\"')))
		(*ut)->len++;
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

char	*put_ctn_hd(char *arg, t_utils *ut, t_arg *ag, t_shell *dt)
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
		buffer = char_til_q(buffer, arg + dt->ut->i, &dt->ut, ag);
		ut->i++;
	}
	return (buffer);
}

char	*cast_heredoc(char *ctn, t_arg *arg, t_shell *data)
{
	char	*buffer;
	char	*prompt;
	char	*eof;

	eof = get_last_ctn(arg);
	prompt = ft_strjoin(eof, "\t>");
	while (1)
	{
		if (!ctn)
			ctn = ft_strdup("");
		buffer = readline(prompt);
		if (g_sig == 130)
			return (free(prompt), ft_str_reset(&ctn), NULL);
		if (!buffer)
			return (ft_printf("%s%s\n", NULL_HD, eof), free(prompt), ctn);
		if (ft_strrchr(buffer, '\n'))
			buffer = ft_strn_replace(buffer, ft_strlen_til_char(buffer, '\n'));
		if (ft_strcmp(buffer, eof) == 0)
			return (free(buffer), free(prompt), ctn);
		buffer = ft_strjoining(buffer, "\n");
		ctn = add_to_hd(arg, ctn, buffer, data->envp);
		free(buffer);
		if (!ctn)
			return (free(prompt), ft_strdup(""));
	}
}

int	childoc(t_shell *data, t_mlst *lst, t_arg *arg, int c_fd)
{
	init_signals(S_HEREDOC);
	lst->hd = cast_heredoc(lst->hd, arg, data);
	if (!lst->hd)
		return (close(c_fd), freerdoc(data, lst), 130);
	write(c_fd, lst->hd, ft_strlen(lst->hd) + 1);
	close(c_fd);
	freerdoc(data, lst);
	return (0);
}

char	*heredoc(t_shell *data, t_mlst *lst, t_arg *arg)
{
	int		fd[2];
	int		pid;
	int		status;
	char	*buffer;

	ft_str_reset(&lst->hd);
	if (pipe(fd) == -1)
		return (ft_printf("%e", PIPE_ERR), NULL);
	pid = fork();
	if (pid == -1)
		return (ft_printf("%s", FORK_ERR), NULL);
	closefds(fd, pid);
	if (pid == 0)
		exit(childoc(data, lst, arg, fd[WRITE]));
	waitpid(pid, &status, 0);
	buffer = get_all_file(fd[READ]);
	if (!buffer)
		buffer = ft_strdup("");
	close(fd[READ]);
	data->exitstatus = WEXITSTATUS(status);
	return (buffer);
}
