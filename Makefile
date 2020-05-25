help:
	@echo 'This is a very simple Makefile to build the example'
	@echo 'Type "make build" to build the example.'
	@echo 'Available commands are:'
	@echo 'help:   Shows this message and exits'
	@echo 'build:  build the example'
build:
	@echo 'Building...'
	g++ -std=c++17 -O3 -Wall -Wextra example.cpp -o prog
	@echo 'Done!'
	@echo 'Type "./prog" to run the example'
	@echo ''
