# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -I/usr/local/include -I./external/googletest/googletest/include -fprofile-arcs -ftest-coverage
LDFLAGS = -fprofile-arcs -ftest-coverage
LIBS = -lsfml-graphics -pthread

# Directories
SRCDIR = ..
BUILDDIR = build
BINDIR = bin
TESTDIR = .

# Source files
SRCS=$(SRCDIR)/BombLogic.cpp $(SRCDIR)/BoardLogic.cpp
TEST_SRCS = $(wildcard $(TESTDIR)/*.cpp)
OBJS = $(SRCS:$(SRCDIR)/%.cpp=$(BUILDDIR)/%.o)
TEST_OBJS = $(TEST_SRCS:$(TESTDIR)/%.cpp=$(BUILDDIR)/%.o)

# Executable
TARGET = $(BINDIR)/test_suite

# Rules
all: $(TARGET)

$(TARGET): $(OBJS) $(TEST_OBJS)
	@mkdir -p $(BINDIR)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^ external/googletest/build/lib/libgtest.a external/googletest/build/lib/libgtest_main.a $(LIBS)

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILDDIR)/%.o: $(TESTDIR)/%.cpp
	@mkdir -p $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

test: $(TARGET)
	./$(TARGET)
	lcov --capture --directory . --output-file coverage.info

clean:
	rm -rf $(BUILDDIR) $(BINDIR) $(TARGET) *.gcda *.gcno *.gcov coverage.info out
.PHONY: all coverage clean
