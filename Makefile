# Email: yedidea9@gmail.com
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Werror -pedantic

.PHONY: Main test valgrind clean

Main:
	$(CXX) $(CXXFLAGS) Main.cpp -o Main
	./Main

test:
	$(CXX) $(CXXFLAGS) Test.cpp -o test_runner
	./test_runner

valgrind:
	$(CXX) $(CXXFLAGS) Test.cpp -o test_runner
	valgrind --leak-check=full \
	         --show-leak-kinds=all \
	         --track-origins=yes \
	         --error-exitcode=1 ./test_runner

clean:
	rm -f Main Main.exe test_runner test_runner.exe *.o *.obj *.tmp
