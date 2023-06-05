# C:\Users\ryoga\Desktop\Code\Reversi

OBJ = Interface.o Player.o ReversiBoard.o

a: $(OBJ)
	g++ $(OBJ)
Interface.o: Interface.cpp
	g++ -c Interface.cpp
ReversiBoard.o: ReversiBoard.cpp
	g++ -c ReversiBoard.cpp 
Player.o: Player.cpp
	g++ -c Player.cpp
.PHONY: clean
clean:
	del *.o