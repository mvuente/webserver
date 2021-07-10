NAME		= webserv
CC			= clang++
INC_DIR		= ..
SRC_DIR		= ..
DEBUG		= -D DEBUG
SMAKE		= make -s
SRC_EXT		= cpp
OBJ_EXT		= o
WWWF		= -Wall -Wextra -Werror -std="c++98"
CFLAGS		= $(WWWF) -I$(INC_DIR)

#====================================================================================
SRCS	    = $(wildcard $(SRC_DIR)/*.$(SRC_EXT))
MAIN		= $(OBJ_DIR)/main.o
#====================================================================================

test_cgi:
			$(CC) $(CFLAGS) test_CGI.cpp ../../core/Request.cpp -I../../core -I../../utils ../CGI.cpp -o test_cgi.out
			./test_cgi.out
			@rm test_cgi.out

test_parser:
			$(CC) $(CFLAGS)  test_Parser.cpp ../HTTPRequestParser.cpp ../Request.cpp -o test_parser.out
			./test_parser.out
			@rm test_parser.out

test_mvuente_request:
			$(CC) $(WWWF) test_Mvuente_Request.cpp ../../core/Request.cpp -I../../core -o test_mvuente_request.out
			./test_mvuente_request.out
			@rm test_mvuente_request.out

clean:
			rm -rf $(OBJ_DIR)
			rm -rf $(TMP_LIB) 

fclean:		clean
			rm -rf $(NAME)

re:			fclean all
