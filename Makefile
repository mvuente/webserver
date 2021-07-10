NAME		= webserv
CC			= clang++
UTILS_DIR	= ./utils
INC_DIR		= ./core ./cgi ./server ./autoindex ./parser $(UTILS_DIR)
SRC_DIR		= .
OBJ_DIR		= bin
DEPS		= ./utils/Logger.hpp
DEBUG		= -D DEBUG
SMAKE		= make -s
AR			= ar rc
TMP_LIB		= $(OBJ_DIR)/libtemp.a
DEP_EXT		= hpp
SRC_EXT		= cpp
OBJ_EXT		= o
IFLAGS		= $(foreach d, $(INC_DIR), -I$d)
CFLAGS		= -Wall -Wextra -Werror -std="c++98" -O3 $(IFLAGS)
DIRS		= $(shell find . -maxdepth 1 -type d)
#====================================================================================
vpath %.$(SRC_EXT) $(DIRS)

TEMP		= $(wildcard *.$(SRC_EXT)) $(wildcard */*.$(SRC_EXT))
#IGNORE		=
#SRCS	    = $(patsubst $(IGNORE),, $(TEMP))
SRCS	    = $(TEMP)
OBJS		= $(patsubst %, $(OBJ_DIR)/%, $(notdir $(SRCS:.$(SRC_EXT)=.$(OBJ_EXT))))
MAIN		= $(OBJ_DIR)/main.o
#====================================================================================

.PHONY:		all clean fclean re test woflags

all:		$(NAME)

#=== Additional dependencies ===

$(MAIN): main.cpp parser/Config.hpp core/Dispatcher.hpp

$(OBJ_DIR)/CGI.o: cgi/CGI.hpp cgi/URL.hpp utils/path.hpp core/Request.hpp core/Response.hpp

$(OBJ_DIR)/ServerMachine.o: core/ServerMachine.hpp core/Request.hpp core/Response.hpp core/HTMLTemplate.hpp utils/path.hpp autoindex/Autoindex.hpp server/Server.hpp
 
$(OBJ_DIR)/Request.o: core/Request.hpp parser/Config.hpp

$(OBJ_DIR)/Response.o: core/Response.hpp core/Request.hpp utils/date.hpp

$(OBJ_DIR)/StatusCode.o: core/StatusCode.hpp

$(OBJ_DIR)/Dispatcher.o: core/Dispatcher.hpp core/ClientsPool.hpp parser/Config.hpp

$(OBJ_DIR)/Config.o: parser/Config.hpp server/Server.hpp

$(OBJ_DIR)/Server.o: server/Server.hpp server/Location.hpp

$(OBJ_DIR)/Location.o: server/Location.hpp

$(OBJ_DIR)/Client.o: core/Client.hpp core/ServerMachine.hpp server/Server.hpp core/Request.hpp core/Response.hpp core/IO.hpp

$(OBJ_DIR)/ClientsPool.o: core/ClientsPool.hpp core/Client.hpp

#===============================

$(OBJ_DIR)/%.$(OBJ_EXT):	%.$(SRC_EXT) $(DEPS)
							$(CC) $(CFLAGS) -c $< -o $@


$(TMP_LIB): $(OBJS)
			$(AR) $@ $?

$(NAME):	$(TMP_LIB)
			$(CC) $(CFLAGS) $(MAIN) -o $(NAME) $(TMP_LIB)

$(OBJS):	| $(OBJ_DIR)

$(OBJ_DIR):
			mkdir -p $(OBJ_DIR)

woflags:	CFLAGS = $(IFLAGS) $(DEBUG)
woflags:	$(NAME)

test:		CFLAGS += $(DEBUG)
test:		$(NAME)
			./$(NAME)

clean:
			rm -rf $(OBJ_DIR)

fclean:		clean
			rm -rf $(NAME) 

re:			fclean all
