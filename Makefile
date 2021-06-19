NAME = lehinserv
CC = clang++
FLAGS = -std=c++98
#FLAGS=
SRC = main1.cpp \
Request.cpp \
Response.cpp \
ServerMachine.cpp \
Dispatcher.cpp


OBJ = $(SRC:.cpp=.o)

.PHONY: all clean fclean re

all: $(NAME)

%.o:%.cpp
	$(CC) -g $(FLAGS) -c $< -o $@

$(NAME): $(OBJ)
	$(CC) $(FLAGS) $(OBJ) -o $(NAME)

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all