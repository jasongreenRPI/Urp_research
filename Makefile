# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -g

# Directories
SRC_DIR = ./CLASSES
TEST_DIR = ./TESTS
BIN_DIR = ./bin
DATA_DIR = ./DATA
DOCS_DIR = ./DOCS

# Source and test files
GRAPH_SRC = $(SRC_DIR)/Graph/Graph.cpp
GRAPH2_HEADERS = $(SRC_DIR)/Graph2/Graph2.h
COMMUNITY_HEADERS = $(SRC_DIR)/Community/Community.h
COMMUNITY_COMPARISON_HEADERS = $(SRC_DIR)/CommunityComparison/CommunityComparison.h

GRAPH2_TEST = $(TEST_DIR)/Graph2_test.cpp
COMMUNITY_TEST = $(TEST_DIR)/Community_test.cpp
COMMUNITY_COMPARISON_TEST = $(TEST_DIR)/CommunityComparison_test.cpp
COMMUNITY_COMPARISON_BENCHMARK_TEST = $(TEST_DIR)/CommunityComparison_benchmark_test.cpp

# Executables
GRAPH2_TEST_BIN = $(BIN_DIR)/graph2_test
COMMUNITY_TEST_BIN = $(BIN_DIR)/community_test
COMMUNITY_COMPARISON_TEST_BIN = $(BIN_DIR)/community_comparison_test
COMMUNITY_COMPARISON_BENCHMARK_BIN = $(BIN_DIR)/community_comparison_benchmark_test
MAIN_BIN = $(BIN_DIR)/main

# Define all targets
all: dirs tests main

# Create necessary directories if they don't exist
dirs:
	mkdir -p $(BIN_DIR)
	mkdir -p $(DOCS_DIR)

# Build and run all tests
tests: graph2_test community_test community_comparison_test community_comparison_benchmark_test

# The main executable
main: dirs
	$(CXX) $(CXXFLAGS) -o $(MAIN_BIN) index.cpp $(GRAPH_SRC)

# Graph2 tests
graph2_test: dirs $(GRAPH2_TEST) $(GRAPH2_HEADERS) $(COMMUNITY_HEADERS)
	$(CXX) $(CXXFLAGS) -o $(GRAPH2_TEST_BIN) $(GRAPH2_TEST)

# Community tests
community_test: dirs $(COMMUNITY_TEST) $(COMMUNITY_HEADERS) $(GRAPH2_HEADERS)
	$(CXX) $(CXXFLAGS) -o $(COMMUNITY_TEST_BIN) $(COMMUNITY_TEST)

# CommunityComparison tests
community_comparison_test: dirs $(COMMUNITY_COMPARISON_TEST) $(COMMUNITY_COMPARISON_HEADERS) $(COMMUNITY_HEADERS)
	$(CXX) $(CXXFLAGS) -o $(COMMUNITY_COMPARISON_TEST_BIN) $(COMMUNITY_COMPARISON_TEST)

# CommunityComparison benchmark tests
community_comparison_benchmark_test: dirs $(COMMUNITY_COMPARISON_BENCHMARK_TEST) $(COMMUNITY_COMPARISON_HEADERS) $(COMMUNITY_HEADERS)
	$(CXX) $(CXXFLAGS) -o $(COMMUNITY_COMPARISON_BENCHMARK_BIN) $(COMMUNITY_COMPARISON_BENCHMARK_TEST)

# Run the tests
run_tests: tests
	@echo "Running Graph2 tests..."
	$(GRAPH2_TEST_BIN)
	@echo "\nRunning Community tests..."
	$(COMMUNITY_TEST_BIN)
	@echo "\nRunning CommunityComparison tests..."
	$(COMMUNITY_COMPARISON_TEST_BIN)
	@echo "\nRunning CommunityComparison benchmark tests..."
	$(COMMUNITY_COMPARISON_BENCHMARK_BIN)

# Run main program
run: main
	$(MAIN_BIN)

# Clean up
clean:
	rm -rf $(BIN_DIR)

.PHONY: all dirs tests main graph2_test community_test community_comparison_test community_comparison_benchmark_test run_tests run clean