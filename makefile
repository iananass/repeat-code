Proj=RepCode

all: $(Proj)

$(Proj): repeat-code.cpp
	g++ -o $@ $< -std=c++0x -pthread -O3

clean:
	rm -rf $(Proj)