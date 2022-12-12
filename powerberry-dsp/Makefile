Files=./src/*.cpp ./src/spi/*.cpp ./src/adc/*.cpp ./src/filters/*.cpp ./src/controller/*.cpp ./src/datastorage/*.cpp ./src/config_manager/*.cpp
Libraries= -l bcm2835 /usr/local/lib/libInfluxDB.so
Includes=./src
.PHONY : ./bin/powerberry-dsp clean

./bin/powerberry-dsp:
	mkdir -p bin
	g++ -g -Wall -Wno-psabi -Wfatal-errors -std=c++17 -pthread -o ./bin/powerberry-dsp $(Files) $(Libraries) -I $(Includes)
	chmod +x ./bin/powerberry-dsp

clean:
	rm -rf ./bin
