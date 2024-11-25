CXX = g++
CXXFLAGS = -I./include -I./lib/SeisMath/include -Wall -fopenmp -O3
OUT = main.out

MAIN_SRC = main.cpp
APP_SRC = $(wildcard src/*.cpp)
LIB_SRC = $(wildcard lib/SeisMath/src/*.cpp)

OBJ = $(SRC:.cpp=.o)

SRC = $(MAIN_SRC) $(APP_SRC) $(LIB_SRC) 

run:
	@$(CXX) $(SRC) $(CXXFLAGS) -o $(OUT) -lm
	@./$(OUT)

plot:
	python3 src/plot/plot.py

cleanObj:
	rm -f $(OBJ) $(OUT)

cleanSnapshots:
	rm -f data/snapshots/*.bin

