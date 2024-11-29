CXX = g++
CXXFLAGS = -I./include -I./lib/SeisMath/include/functions -Wall -fopenmp -O3
LDFLAGS = -Llib/SeisMath -lSeisMath
OUT = main.out

MAIN_SRC = main.cpp
APP_SRC = $(wildcard src/*.cpp)

OBJ = $(SRC:.cpp=.o)

SRC = $(MAIN_SRC) $(APP_SRC) $(LIB_SRC) 

run:
	@$(CXX) $(SRC) $(CXXFLAGS) $(LDFLAGS) -o $(OUT) -lm
	@./$(OUT)

plot:
	python3 src/plot/plot.py

cleanObj:
	rm -f $(OBJ) $(OUT)

cleanSnapshots:
	rm -f data/snapshots/*.bin

