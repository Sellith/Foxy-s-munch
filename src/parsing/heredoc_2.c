/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvan-bre <lvan-bre@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 22:18:30 by lvan-bre          #+#    #+#             */
/*   Updated: 2025/05/21 05:59:22 by lvan-bre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.functions.h"

/*
	Expand environment variable at start of 'raw' and append to 'cooked'.
	If no variable name, append literal '$'.
	Look up variable in 'env', append its value or empty string if not found.
	Update '*x' to skip parsed variable name.
	Return new string or NULL on allocation failure.
*/
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

/*
	This function duplicates characters from 'raw' up to but not including the
	next $ character, unless inside quotes (arg->q_trace = true), in which case
	it duplicates the entire string.
	It appends the duplicated substring to the 'cooked' string.
	The index pointed by x is incremented by the number of characters processed.

	If memory allocation fails at any point, it returns NULL.
	Otherwise, it returns the newly concatenated string.
*/
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

/*
	This function adds processed content from 'sent' to the existing heredoc
	line buffer.
	It handles environment variable expansion unless inside quotes.
	It iterates over the characters of 'sent', expanding variables or copying
	literals accordingly.
	If any memory allocation fails during the process, it returns NULL.
	If the final buffer is empty, it appends a space to the line.
	Returns the new concatenated line or NULL on failure.
*/
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
