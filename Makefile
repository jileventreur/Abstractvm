CC = clang++
CFLAGS = -Werror -Wall -Wextra -std=c++14

NAME = avm

DIR_SRC = ./srcs/
INCLUDES = -I ./headers -I /Users/cadam/.brew/Cellar/boost/1.66.0/include/

FILES = main.cpp \
        Operand.cpp \
        Factory.cpp \
        MyException.cpp \
        CustomStack.cpp \
        Lexer.cpp \
        Parser.cpp \
        VM.cpp \

SRC := $(addprefix $(DIR_SRC), $(FILES))

OBJ = $(SRC:%.cpp=%.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ) $(LIBS)

%.o: %.cpp
	$(CC) $(CFLAGS) -o $@ -c $< $(INCLUDES)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re : fclean all

.PHONY: all clean fclean re