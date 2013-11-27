Proj=RepCode

all: $(Proj)

$(Proj): repeat-code.cpp
	g++ -o $@ $<

clean:
	rm -rf $(Proj)