# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -fprofile-arcs -ftest-coverage
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
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS) external/googletest/build/lib/libgtest.a external/googletest/build/lib/libgtest_main.a 

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
	lcov --capture --directory . --output-file coverage.info
	lcov --remove coverage.info '/usr/*' --output-file coverage.info
	lcov --remove coverage.info '/external/*' --output-file coverage.info
	genhtml coverage.info --output-directory out

coveralls: coverage
	lcov --list coverage.info
	cat coverage.info
	coveralls -t zvmMgms0er7IPOhnNrF3jaBzeVDIJbHFy --root . -E ".*external.*" -E ".*tests.*" -i out/coverage.info
	cat coverage.info | cpp-coveralls --verbose --repo-token zvmMgms0er7IPOhnNrF3jaBzeVDIJbHFy -E ".*external.*" -E ".*tests.*" -i out/coverage.info


.PHONY: all clean