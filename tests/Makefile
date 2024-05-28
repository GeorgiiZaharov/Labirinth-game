# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -fprofile-arcs -ftest-coverage -Wall -I/usr/local/include -I./external/googletest/googletest/include
LIBS = -lsfml-graphics -pthread

# Directories
SRCDIR = ..
BUILDDIR = build
BINDIR = bin
TESTDIR = .

SRCS=$(SRCDIR)/BoardLogic.cpp $(SRCDIR)/BombLogic.cpp
TEST_SRCS = $(wildcard $(TESTDIR)/*.cpp)
OBJS = $(SRCS:$(SRCDIR)/%.cpp=$(BUILDDIR)/%.o)
TEST_OBJS = $(TEST_SRCS:$(TESTDIR)/%.cpp=$(BUILDDIR)/%.o)

# Executable
TARGET = $(BINDIR)/test_suite

# Rules
all: $(TARGET)

$(TARGET): $(OBJS) $(TEST_OBJS)
	@mkdir -p $(BINDIR)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS) ./external/googletest/build/lib/libgtest.a ./external/googletest/build/lib/libgtest_main.a 

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILDDIR)/%.o: $(TESTDIR)/%.cpp
	@mkdir -p $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILDDIR) $(BINDIR) $(TARGET) *.gcda *.gcno *.gcov coverage.info out

test: $(TARGET)
	./$(TARGET)

coverage: $(TARGET)
	./$(TARGET)
	lcov --capture --directory ./build --output-file coverage.info
	lcov --remove coverage.info '/usr/*' --output-file coverage.info
	lcov --remove coverage.info '*/external/*' --output-file coverage.info
	lcov --remove coverage.info '*/tests/*' --output-file coverage.info

	lcov --list coverage.info
coveralls: coverage
	coveralls --gcov-options '\-lp' --root .. --build-root . -E ".*external.*" -E ".*tests.*" -t zvmMgms0er7IPOhnNrF3jaBzeVDIJbHFy 


.PHONY: all clean