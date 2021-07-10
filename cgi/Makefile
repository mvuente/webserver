NAME		= webserv
CC			= clang++
INC_DIR		= .
SRC_DIR		= .
OBJ_DIR		= bin
DEPS		= 
UTILS_DIR	= ./utils
DEBUG		= -D DEBUG
SMAKE		= make -s
AR			= ar rc
TMP_LIB		= libtemp.a
SRC_EXT		= cpp
OBJ_EXT		= o
CFLAGS		= -Wall -Wextra -Werror -std="c++98" -I$(INC_DIR) -I$(UTILS_DIR)

#====================================================================================
UTILS_SRCS	= $(wildcard $(UTILS_DIR)/*.$(SRC_EXT))
SRCS	    = $(wildcard $(SRC_DIR)/*.$(SRC_EXT))
SRCS	   += $(UTILS_SRCS)
OBJS		= $(patsubst $(SRC_DIR)/%, $(OBJ_DIR)/%, $(SRCS:.$(SRC_EXT)=.$(OBJ_EXT)))
MAIN		= $(OBJ_DIR)/main.o
#====================================================================================

.PHONY:		all clean fclean re test

$(OBJ_DIR)/%.$(OBJ_EXT):	$(SRC_DIR)/%.$(SRC_EXT) $(DEPS)
							$(CC) $(CFLAGS) -c $< -o $@

all:		$(NAME)

$(TMP_LIB): $(OBJS)
			$(AR) $@ $?

$(NAME):	$(TMP_LIB)
			$(CC) $(CFLAGS) $(MAIN) -o $(NAME) $(TMP_LIB)

$(OBJS):	| $(OBJ_DIR)

$(OBJ_DIR):
			mkdir -p $(OBJ_DIR)

#=== Additional dependencies ===
$(OBJ_DIR)/Request.o:	Request.hpp Header.hpp URL.hpp
$(OBJ_DIR)/HTTPRequestParser.o:	Request.hpp Client.hpp
$(MAIN):				Router.hpp
#===============================

test:		CFLAGS += $(DEBUG)
test:		$(NAME)
			./$(NAME)

clean:
			rm -rf $(OBJ_DIR)
			rm -rf $(TMP_LIB) 

fclean:		clean
			rm -rf $(NAME)

re:			fclean all
