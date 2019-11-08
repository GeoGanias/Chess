CC=g++
CFLAGS=-Wall
DEPS=ChessInterface.hpp
OBJ=ChessImplementation.o game.o

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

game: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	$(RM) $(OBJ) game

	
