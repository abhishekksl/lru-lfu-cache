CXX      := g++
CXXFLAGS := -std=c++17 -O3 -march=native -Wall -Wextra -I src/include
TARGET   := cache_benchmark
SRC      := src/main.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)
