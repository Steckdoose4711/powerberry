
Files=./src/*.cpp 
./bin/powerberry-dsp:
	mkdir -p bin
	g++ -g -Wall -Wfatal-errors -std=c++17 -pthread -o ./bin/powerberry-dsp $(Files) /usr/local/lib/libredis++.a /usr/local/lib/libhiredis.a -l bcm2835 
	#g++ -g -Wall -Wfatal-errors -std=c++17 -pthread -o ./bin/powerberry-dsp $(Files) -l bcm2835 
	chmod +x ./bin/powerberry-dsp

clean:
	rm -rf ./bin
