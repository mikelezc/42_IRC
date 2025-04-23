# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fdiaz-gu <fdiaz-gu@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/22 10:57:27 by mlezcano          #+#    #+#              #
#    Updated: 2024/10/08 11:42:42 by fdiaz-gu         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Compiler
CC          =   c++
RM          =   rm -rf
DEBUG       =   -fsanitize=address -g3
CFLAGS      =   -Wall -Wextra -Werror -std=c++98 $(DEBUG) -I ./inc -I ./src/commands -I ./src/bonus

# Output
NAME        =   ircserv
NAME_BONUS  =   bot

# Directories 
SRC_DIR     =   ./src/
BONUS_DIR   =   ./bonus/
OBJ_DIR     =   ./obj/
INC_DIR     =   ./inc/

# Sources
SRC         =   main.cpp \
                server/Server.cpp \
				server/Signal.cpp \
				server/ServerSocket.cpp \
				server/AddClient.cpp \
				server/Input.cpp \
				server/GetSet.cpp \
				server/Erase.cpp \
				server/Disconnect.cpp \
				server/CmdParser.cpp \
                client/Client.cpp \
				client/GetSet.cpp \
                channel/Channel.cpp \
				channel/Getters.cpp \
				channel/Setters.cpp \
				channel/SendToAll.cpp \
				commands/Command.cpp \
				commands/Pass.cpp \
				commands/Nick.cpp \
				commands/User.cpp \
				commands/Invite.cpp \
				commands/Join.cpp \
				commands/Kick.cpp \
				commands/Mode.cpp \
				commands/Part.cpp \
				commands/Privmsg.cpp \
				commands/Quit.cpp \
				commands/Topic.cpp

SRC_BONUS   =   main.cpp \
                player.cpp \
                bot.cpp \
                
SRCS        =   $(addprefix $(SRC_DIR), $(SRC))
SRCS_BONUS  =   $(addprefix $(BONUS_DIR), $(SRC_BONUS))

OBJ         =   $(SRCS:.cpp=.o)
OBJS        =   $(addprefix $(OBJ_DIR), $(OBJ))
OBJ_BONUS   =   $(SRCS_BONUS:.cpp=.o)
OBJS_BONUS  =   $(addprefix $(OBJ_DIR), $(OBJ_BONUS))

# Colors
DEF_COLOR   =   \033[0;39m
ORANGE      =   \033[0;33m
GRAY        =   \033[0;90m
RED         =   \033[0;91m
GREEN       =   \033[1;92m
YELLOW      =   \033[1;93m
BLUE        =   \033[0;94m
MAGENTA     =   \033[0;95m
CYAN        =   \033[0;96m
WHITE       =   \033[0;97m

# Make rules
TOTAL_SRCS  =   $(words $(SRCS))
COMPILE_CNT =   0

all: $(OBJ_DIR) $(NAME)

$(OBJ_DIR):
		@mkdir -p $(OBJ_DIR)
		@mkdir -p $(OBJ_DIR)src
		@mkdir -p $(OBJ_DIR)src/server
		@mkdir -p $(OBJ_DIR)src/client
		@mkdir -p $(OBJ_DIR)src/commands
		@mkdir -p $(OBJ_DIR)src/channel
		@mkdir -p $(OBJ_DIR)bonus

$(OBJ_DIR)%.o: %.cpp | $(OBJ_DIR)
		@$(CC) $(CFLAGS) -I $(INC_DIR) -c $< -o $@
		@$(eval COMPILE_CNT=$(shell echo $$(($(COMPILE_CNT)+1))))
		@printf "$(GRAY)[$(NAME)]$(DEF_COLOR) $(DEF_COLOR)$(GREEN) [%s] [%s]\r" \
			"$(shell echo $$((($(COMPILE_CNT)*100)/$(TOTAL_SRCS))))%" \
			"$(shell seq -s '#' $(shell echo $$((($(COMPILE_CNT)*20)/$(TOTAL_SRCS)))) | tr -d '[:digit:]')"

$(NAME): $(OBJS)
		@$(CC) $(CFLAGS) -I $(INC_DIR) -o $(NAME) $(OBJS)
		@printf "$(GREEN)[$(NAME)]\n$(DEF_COLOR)"

bonus: $(OBJS_BONUS)
		@$(CC) $(CFLAGS) -I $(INC_DIR) -o $(NAME_BONUS) $(OBJS_BONUS)
		@printf "$(GREEN)[$(NAME_BONUS)]\n$(DEF_COLOR)"

clean:
		@$(RM) $(OBJ_DIR)
		@echo "$(CYAN)objects...     $(BLUE)cleaned! $(DEF_COLOR)"

fclean: clean
		@$(RM) $(NAME) $(NAME_BONUS)
		@echo "$(CYAN)executables...  $(BLUE)cleaned! $(DEF_COLOR)"

re:		fclean all

# Valgrind:
# To test memory leaks with valgrind
# Example: make valgrind ARGS="2345 hello"

valgrind: CFLAGS := $(filter-out -fsanitize=address, $(CFLAGS))
valgrind: fclean $(NAME)
		@$(eval ARGS := $(if $(ARGS),$(ARGS),))
		@if [ -z "$(ARGS)" ]; then \
			echo "$(YELLOW)Warning: Running without arguments."; \
			echo "To pass arguments, use: 'make valgrind ARGS=\"<your arguments>\"'$(DEF_COLOR)"; \
		fi
		@valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(NAME) $(ARGS)

valgrind_bonus: CFLAGS := $(filter-out -fsanitize=address, $(CFLAGS))
valgrind_bonus: fclean bonus
		@$(eval ARGS := $(if $(ARGS),$(ARGS),))
		@if [ -z "$(ARGS)" ]; then \
			echo "$(YELLOW)Warning: Running without arguments.$(DEF_COLOR)"; \
			echo "To pass arguments, use: 'make valgrind_bonus ARGS=\"<your arguments>\"'"; \
		fi
		@valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(NAME_BONUS) $(ARGS)

.PHONY: all clean fclean re bonus valgrind valgrind_bonus
