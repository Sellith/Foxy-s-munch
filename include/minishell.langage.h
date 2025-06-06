/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.langage.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvan-bre <lvan-bre@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 21:31:29 by lvan-bre          #+#    #+#             */
/*   Updated: 2025/05/22 01:25:35 by lvan-bre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_LANGAGE_H
# define MINISHELL_LANGAGE_H

/* Parsing error messages */

# define ERR_ARGS "\033[1;91mFoxy's munch: error: Minishell takes no \
args\n\033[0m"
# define TOK_ERR "\033[1;91mFoxy's munch: error: Syntax error\n\033[0m"
# define PARS_ERR "\033[1;91mFoxy's munch: error: Parsing error \n\033[0m"
# define PIPE_ERR "\033[1;91mFoxy's munch: error: Pipe error\n\033[0m"
# define FORK_ERR "\033[1;91mFoxy's munch: error: Fork error\n\033[0m"
# define ENVP_ERR "\033[1;91mFoxy's munch: error: Couldn't fetch environnement, \
abort\n\033[0m"
# define QUOTE_ERR "\033[1;91mFoxy's munch: error: Unclosed quotes not supported \
 by [Foxy's Munch]\n\033[0m"
# define RDIR_ERR "\033[0;91mFoxy's munch: No such file or directory\n\033[0m"
# define NULL_HD "\033[0;93mFoxy's munch: Warning: here_document delimited by \
end-of-file wanted : \033[0m"

/* Exec error messages */

# define PERM_ERR ": Permission denied"
# define NOTF_ERR ": Command not found"
# define INV_ERR ": Invalid process"

/* build ins error messages */

# define IDENTIFIER_ERR "Foxy's munch: export: not an identifier: "
# define ECHO_WRITE_ERR "Foxy's munch: echo: write error"
# define ENV_WRITE_ERR "Foxy's munch: env: write error"
# define EXPORT_WRITE_ERR "Foxy's munch: export: write error"
# define EXP_OPT_ERR "Foxy's munch: export takes no options\nenv: \
usage: export [name ...]"
# define PWD_WRITE_ERR "Foxy's munch: pwd: write error"
# define CD_STD_ERR "Foxy's munch: cd: "
# define ENV_ARG_ERR "Foxy's munch: env takes no arguments\nenv: usage: env"
# define ENV_OPT_ERR "Foxy's munch: env takes no options\nenv: usage: env"
# define PWD_OPT_ERR "Foxy's munch: pwd takes no options\npwd: usage: pwd"
# define CD_NOT_IN_PWD "Foxy's munch: cd: no such file or directory: "
# define CD_ARGS_ERR "Foxy's munch: cd: too many arguments"
# define CD_OLD_NOT_SET_ERR "Foxy's munch: cd: OLDPWD not set"
# define CD_NO_HOME_ERR "Foxy's munch: cd: HOME not set"
# define UNSET_OPT_ERR "Foxy's munch: unset takes no option\nunset: \
usage: unset [name ...]"
# define EXIT_ERR "Foxy's munch: exit: "
# define EXIT_NUM_ERR ": numeric argument required"
# define EXIT_ARGS_ERR "Foxy's munch: exit: too many arguments"

/* colour definition */

# define RED "\001\033[0;91m\002"
# define GRN "\001\033[0;92m\002"
# define YLW "\001\033[0;93m\002"
# define BLUE "\001\033[0;94m\002"
# define PPL "\001\033[0;95m\002"
# define CYAN "\001\033[0;96m\002"
# define WTH "\001\033[0;97m\002"
# define RESET "\001\033[0m\002"

#endif
