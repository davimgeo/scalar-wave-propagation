CXX = g++
CXXFLAGS = -I./include -Wall -fopenmp -O3 
OUT = main.out

MAIN_SRC = main.cpp
APP_SRC = $(wildcard src/*.cpp)

OBJ = $(SRC:.cpp=.o)

SRC = $(MAIN_SRC) $(APP_SRC) 

run:
	@$(CXX) $(SRC) $(CXXFLAGS) $(LDFLAGS) -o $(OUT) -lm
	@./$(OUT)

plot:
	python3 plot.py

cleanObj:
	rm -f $(OBJ) $(OUT)

cleanSnapshots:
	rm -f data/snapshots/*.bin

