/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.defines.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvan-bre <lvan-bre@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 22:40:43 by lvan-bre          #+#    #+#             */
/*   Updated: 2025/05/21 06:26:27 by lvan-bre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_DEFINES_H
# define MINISHELL_DEFINES_H

typedef struct s_pid	t_pid;
typedef struct s_mlst	t_mlst;
extern int				g_sig;

typedef enum e_setsig
{
	S_INTERACTIVE,
	S_HEREDOC,
	S_IGNORE,
	S_DEFAULT,
}	t_setsig;

typedef enum e_btins
{
	BINARY,
	BT_EXIT,
	BT_ECHO,
	BT_ENV,
	BT_CD,
	BT_PWD,
	BT_EXPORT,
	BT_UNSET,
	BT_NULL,
}	t_btins;

typedef enum e_in_out
{
	INF,
	OUT,
	CMD,
}	t_io;

typedef enum e_rdir
{
	NUL,
	SING,
	DOUB,
}	t_rdir;

enum e_pipe
{
	READ,
	WRITE,
}	;

typedef struct s_utils
{
	char	c;
	int		i;
	int		j;
	int		tmp;
	int		ret;
	int		len;
	int		size;
	char	*buff;
	char	*tmpc;
}			t_utils;

typedef struct s_ctn
{
	char	**cmd_ctn;
	int		inf_fd;
	int		outf_fd;
}			t_ctn;

typedef struct s_arg
{
	void	*next;
	char	*ctn;
	int		sq;
	int		dq;
	bool	q;
	bool	q_trace;
}			t_arg;

typedef struct s_mlst
{
	t_mlst	*next;
	t_mlst	*prev;
	t_rdir	rdir[2];
	bool	lstcmd;
	bool	b_in;
	char	*hd;
	int		hd_c;
	t_ctn	*ctn;
	t_arg	*inf;
	t_arg	*append;
	t_arg	*trunc;
	t_arg	*eof;
	t_arg	*cmd;
}			t_mlst;

typedef struct s_pid
{
	t_pid	*next;
	int		pid;
}			t_pid;

typedef struct s_shell
{
	char	**envp;
	char	**path;
	char	*line;
	int		lsize;
	int		pipes;
	int		exitstatus;
	int		stdin_clone;
	char	*home;
	char	*prompt;
	char	*pwd;
	char	*old_pwd;
	t_pid	*pid;
	t_mlst	*mlst;
	t_utils	*ut;
}			t_shell;

#endif
