inoclient: inoclient.cpp tmp/PigpioMS.o
	mkdir -p bin
	g++ -Wall -o bin/inoclient inoclient.cpp tmp/PigpioMS.o -lpigpio -std=c++17 -pthread -lrt
PigpioMS.o: external/PigpioMS/PigpioMS.cpp
	mkdir -p tmp
	g++ -Wall -c -o tmp/PigpioMS.o external/PigpioMS/PigpioMS.cpp -lpigpio -std=c++17 -pthread -lrt
clean:
	rm -rf bin tmp
