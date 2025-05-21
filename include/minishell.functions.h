/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.functions.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvan-bre <lvan-bre@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 21:20:22 by lvan-bre          #+#    #+#             */
/*   Updated: 2025/05/21 23:07:43 by lvan-bre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_FUNCTIONS_H
# define MINISHELL_FUNCTIONS_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdbool.h>
# include <sys/wait.h>
# include <errno.h>
# include <limits.h>
# include <linux/limits.h>

# include "libft.h"
# include "minishell.defines.h"
# include "minishell.langage.h"

/*initiation*/

bool			initdata(t_shell *data);
bool			initmlst(t_mlst **new);
void			reinit_ut(t_utils *ut);
void			reset_data(t_shell *data, t_mlst **lst);
bool			init_pwd(t_shell *data);

/*utils*/

bool			is_token(char c);
bool			is_envable(char c);
void			exit_sig(t_shell *data);
void			get_home(t_shell *data, char **envp);
char			**get_path(t_shell *data, char **envp);
int				search_env_var(char **envp, char *var);

/*debug functions*/

void			print_lists(t_mlst *lst);

/*parsing*/

void			last_cmd_status(t_shell *data);
bool			parsing(t_shell *dt);
bool			next_pipe(t_shell *dt);
bool			selec_tolkien(t_shell *dt, t_mlst *new, t_rdir type, t_io rdir);
char			*put_ctn(char *arg, t_utils *ut, t_arg *ag, t_shell *dt);
char			*put_ctn_hd(char *arg, t_utils *ut, t_arg *ag, t_shell *dt);
char			*heredoc(t_shell *data, t_mlst *lst, t_arg *arg);
char			*cast_heredoc(char *ctn, t_arg *arg, t_shell *data);
char			*add_to_hd(t_arg *arg, char *line, char *sent, char **env);
int				childoc(t_shell *data, t_mlst *lst, t_arg *arg, int c_fd);
char			*get_last_ctn(t_arg *arg);
char			*rm_from_line(t_shell *dt, char *line);

/*build ins*/

unsigned long	do_export(char ***envp, char *str);
unsigned long	ft_export(t_shell *data, char	***envp, char **cmd);
unsigned long	ft_unset(char **cmd, t_shell *data);
unsigned long	ft_exit(t_shell *data, char **cmd);
unsigned long	ft_pwd(t_shell *data, char **cmd);
unsigned long	ft_cd(char **cmd, t_shell *data);
unsigned long	ft_env(char **cmd, char **env);
unsigned long	ft_echo(char **cmd);

/*signal handling*/

void			init_signals(t_setsig set);

/*exec utils*/

char			**ft_ltoda(t_arg *cmd);
void			talk_back(t_pid **pid, t_pid *new);
t_pid			*new_pid(int new_pid);
t_btins			is_b_in(char *cmd);

/* error handling */

void			errhdlg(t_shell *data, t_ctn *ctn, unsigned long err);

/*executing*/

void			minishell(t_shell *data, t_mlst *lst);
void			exec_cmd(t_shell *data, t_mlst *lst, t_ctn *ctn);
bool			open_allfiles(t_mlst *lst);
bool			open_inf(char *hd, t_rdir in_rdir, t_mlst *lst, t_ctn *ctn);
void			execbt(t_shell *data, t_mlst *lst, t_ctn *ctn, t_btins cmd);

/*fd closing*/

void			ft_close(int *fd);
void			closefds(int ddlg[2], int pid);
void			close_n_dup(t_mlst *lst, t_ctn *ctn, int p_fd[2], int tmpid);

/*free all*/

void			freelst(t_arg *arg);
void			freemlst(t_mlst *mlst);
void			freerpipe(t_shell *data);
void			freerdoc(t_shell *data, t_mlst *new);

/*program exit*/

void			so_long_minishell(t_shell *data, unsigned long exitstat);
void			so_long_exec(t_shell *data, unsigned long exitstat, char *msg);

#endif
