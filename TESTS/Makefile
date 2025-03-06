CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra

# Graph2 tests
test_graph2: test_graph2.cpp 
	$(CXX) $(CXXFLAGS) -o test_graph2 $^

run_test_graph2: test_graph2
	./test_graph2

# Clean target
clean:
	rm -f test_graph2

.PHONY: run_test_graph2 clean