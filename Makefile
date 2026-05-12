CC=g++
FLAGS=-std=c++20 -Wall -Werror -Wextra
TARGET=media-scanner

.PHONY:all clear rebuild

SRC=src/*.cpp
OBJ=$(SRC:.cpp=.o)

all: $(TARGET)

$(TARGET):$(OBJ)
	$(CC) $(FLAGS) $(OBJ) -o $(TARGET) -lpthread

%.o: %.cpp
	$(CC) $(FLAGS) -c $< -o $@ -lpthread

clang:
	clang-format -i src/*.cpp src/*.h 
style:
	clang-format -n src/*cpp src/*.h
clean:
	rm -rf $(OBJ) $(TARGET)
open:
	xdg-open http://localhost:1234/media_files

rebuild: clean all