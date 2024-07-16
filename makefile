# The target executable name
output: main.o
	g++ -std=c++11 main.o -o output

# Compilation rule for main.cpp
main.o: main.cpp vfs.hpp inode.hpp queue.hpp vector.hpp
	g++ -std=c++11 -c main.cpp

# Clean rule to remove object files and the executable
clean:
	rm main.o output