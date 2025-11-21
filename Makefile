CXX = g++
CXXFLAGS = -O3 -march=native -std=c++17 -pthread
SRC = src/cache_demo.cpp
OUT = cache_demo

all: $(OUT)

$(OUT): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(OUT)

clean:
	rm -f $(OUT)
