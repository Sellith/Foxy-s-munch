# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lvan-bre <lvan-bre@student.42lehavre.fr    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/08 01:08:11 by lvan-bre          #+#    #+#              #
#    Updated: 2025/05/30 00:23:26 by lvan-bre         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# COMPILATION

CC					:=	gcc
CFLAGS				:=	-Wextra -Wall -Werror -g
RLFLAGS				:=	-lreadline
VALFLAGS			:=	--leak-check=full --show-leak-kinds=all --track-fds=yes --trace-children=yes --track-origins=yes --suppressions=/home/lvan-bre/Tronc_commun/Foxy_munch/additional_files/readline.supp

# COMMANDS

RM					:=	rm -f
RMDIR				:=	rm -rf

# DIRS

BIN					:=	bin
BUILD				:=	build
SRC_DIR				:=	src
INC_DIR				:=	include
BUILD_SRC			:=	$(BUILD)/$(SRC_DIR)
DEP					:=	Depedencies
ARCH				:=	$(BIN)/archives

# TARGET

TARGET				:=	minishell
NAME				:=	$(BIN)/$(TARGET)
TARGET_H			:=	minishell.h

# LIBFT

LIBFT_TARGET		:=	libft.a
LIBFT_H				:=	libft.h
LIBFT_DIR			:=	Libft_Updated
LIBFT_PATH			:=	$(DEP)/$(LIBFT_DIR)
LIBFT_H_PATH		:=	$(LIBFT_PATH)/$(LIBFT_H)
FT_PRINTF_H			:=	ft_printf.h
FT_PRINTF_H_PATH	:=	$(LIBFT_PATH)/$(FT_PRINTF_H)
GNL_H				:=	get_next_line.h
GNL_H_PATH			:=	$(LIBFT_PATH)/$(GNL_H)

# INCLUDES & ARCHIVES

ARCHIVES			:=	$(ARCH)/$(LIBFT_TARGET)
INCLUDES			:=	$(INC_DIR)/

# COLORS_DEFINITIONS

CLR_RESET	=	\033[0m
CLR_RED		=	\033[1;91m
CLR_GRN		=	\033[1;92m
CLR_YLW		=	\033[1;93m
CLR_BLUE	=	\033[1;94m
CLR_PPL		=	\033[1;95m
CLR_LPPL	=	\033[1;35m
CLR_CYAN	=	\033[1;96m
CLR_WTH		=	\033[1;97m

ERS_STR		=	\033[2K\r

#	SRC FILES

SRC		=	main.c parsing/put_ctn.c parsing/parsing_utils.c parsing/parsing.c	\
			parsing/to_list.c parsing/initall.c parsing/heredoc.c utils/utils.c	\
			parsing/signal.c parsing/heredoc_2.c utils/debug_functions.c		\
			parsing/next_pipe.c exit_free/exit.c exit_free/free.c exec/exec.c	\
			exec/files.c exec/minishell.c exec/utils.c exec/err_msg.c 			\
			built_in/ft_env.c built_in/ft_echo.c built_in/ft_pwd.c 				\
			built_in/ft_export.c exec/execbt.c built_in/ft_unset.c				\
			built_in/ft_cd.c built_in/ft_exit.c built_in/ft_export_env.c


#	OBJS CONVERSION

OBJS	=	$(patsubst %.c, $(BUILD_SRC)/%.o, $(SRC))

#	RULES

all:	$(NAME)

clean:
	@$(MAKE) clean -C $(LIBFT_PATH) --no-print-directory
	@$(RM) $(OBJS)

fclean: clean
	@$(MAKE) fclean -C $(LIBFT_PATH) --no-print-directory
	@$(RM) $(NAME)

re:	fclean all

cleandep:
	@$(MAKE) clean -C $(LIBFT_PATH) --no-print-directory

fcleandep:
	@$(MAKE) fclean -C $(LIBFT_PATH) --no-print-directory

mkdep:
	@cp $(LIBFT_H_PATH) $(INC_DIR)
	@$(MAKE) -C $(LIBFT_PATH) --no-print-directory

redep: fcleandep mkdep

cleansrc:
	@$(RM) $(OBJS)

fcleansrc: cleansrc
	@$(RM) $(NAME)

resrc: fcleansrc all

val:
	@valgrind $(VALFLAGS) $(NAME)

normy:
	@norminette $(SRC_DIR) $(INC_DIR) $(DEP)/$(LIBFT) | grep -Ev 'Notice|OK!$$' \
	&& echo '$(CLR_RED)Norminette : KO !$(CLR_RESET)'						\
	|| echo '$(CLR_CYAN)Norminette : OK !$(CLR_RESET)'

val_env_less:
	@env -i valgrind $(VALFLAGS) $(NAME)

libft:
	@printf "$(CLR_BLUE)\tInfo:\t\t$(CLR_CYAN)Cloning $(CLR_LPPL)\"%s\"$(CLR_CYAN) into $(CLR_LPPL)\"%s\"...$(CLR_RESET)" $(LIBFT_DIR) $(DEP)
	@git clone -q git@github.com:Sellith/Libft_Updated.git $(LIBFT_PATH)
	@printf "$(ERS_STR)$(CLR_BLUE)\tInfo:\t\t$(CLR_YLW)\"%s\" $(CLR_GRN) Cloned into $(CLR_YLW) \"%s\" !$(CLR_RESET)\n" $(LIBFT_DIR) $(DEP)
	@cp $(LIBFT_H_PATH) $(INC_DIR)
	@cp $(FT_PRINTF_H_PATH) $(INC_DIR)
	@cp $(GNL_H_PATH) $(INC_DIR)

rmlib:
	@$(RMDIR) $(LIBFT_PATH)
	@printf "$(CLR_BLUE)\tInfo:\t\t$(CLR_RED)Removed \"%s\" !$(CLR_RESET)\n" $(LIBFT_DIR)

udlib: fcleandep rmlib libft redep

.PHONY: all clean fclean re cleandep fcleandep mkdep redep cleansrc fcleansrc resrc val normy libft rmlib udlib

# COMPILATION

$(NAME):	$(OBJS)
	@printf "$(ERS_STR)$(CLR_BLUE)\tSource files:\t$(CLR_GRN)Compilation succeded !$(CLR_RESET)\n"
	@$(MAKE) -C $(LIBFT_PATH) --no-print-directory
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) $(OBJS) -I$(INC_DIR) $(RLFLAGS) $(ARCHIVES) -o $@
	@printf "$(ERS_STR)$(CLR_BLUE)\tDone Building:\t$(CLR_LPPL)\"%s\"$(CLR_GRN) in $(CLR_LPPL)\"%s/\"$(CLR_GRN)!$(CLR_RESET)\n" $(TARGET) $(BIN)

$(BUILD_SRC)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) -c $< -o $@ -I$(INC_DIR)
	@printf "$(ERS_STR)$(CLR_CYAN)\tCompiling: $(CLR_PPL)\"%s\"$(CLR_CYAN)..." $<
