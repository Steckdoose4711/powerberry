
Files= ./src/*cpp 
./bin/powerberry-dsp: /redis/redis-plus-plus/build/libredis++.a /redis/hiredis/libhiredis.a -pthread
	mkdir -p bin
	g++ -std=c++17 -o ./bin/powerberry-dsp $(Files)
	chmod +x ./bin/powerberry-dsp

clean:
	rm -rf ./bin
