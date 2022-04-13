Files=./src/*.cpp ./src/spi/*.cpp ./src/adc/*.cpp ./src/filters/*.cpp ./src/controller/*.cpp ./src/datastorage/*.cpp ./src/config_manager/*.cpp
Libraries=/usr/local/lib/libredis++.a /usr/local/lib/libhiredis.a -l bcm2835
Includes=./src
#.PHONY : ./bin/powerberry-dsp

./bin/powerberry-dsp:
	mkdir -p bin
	g++ -g -Wall -Wno-psabi -Wfatal-errors -std=c++17 -pthread -o ./bin/powerberry-dsp $(Files) $(Libraries) -I $(Includes)
	chmod +x ./bin/powerberry-dsp

clean:
	rm -rf ./bin
