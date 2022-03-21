
Files=./src/*.cpp ./src/spi/*.cpp ./src/adc/*.cpp ./src/filters/*.cpp ./src/controller/*.cpp ./src/datastorage/*.cpp
Libraries=/usr/local/lib/libredis++.a /usr/local/lib/libhiredis.a -l bcm2835
Includes= ./src
#.PHONY : ./bin/powerberry-dsp

./bin/powerberry-dsp:
	mkdir -p bin
	g++ -g -Wall -Wno-psabi -Wfatal-errors -std=c++17 -pthread -o ./bin/powerberry-dsp $(Files) $(Libraries) -I $(Includes) -O3
	chmod +x ./bin/pdockerowerberry-dsp

clean:
	rm -rf ./bin
