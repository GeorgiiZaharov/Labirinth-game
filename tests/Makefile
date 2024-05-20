# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -I/usr/local/include -fprofile-arcs -ftest-coverage
LDFLAGS = -fprofile-arcs -ftest-coverage
LIBS = -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -pthread

# Directories
SRCDIR = ..
BUILDDIR = build
TESTDIR = .
GTEST_DIR = external/googletest/build

# Source files
SRCS = $(filter-out $(SRCDIR)/main.cpp, $(wildcard $(SRCDIR)/*.cpp))
TEST_SRCS = $(wildcard $(TESTDIR)/*.cpp)
OBJS = $(SRCS:$(SRCDIR)/%.cpp=$(BUILDDIR)/%.o)
TEST_OBJS = $(TEST_SRCS:$(TESTDIR)/%.cpp=$(BUILDDIR)/%.o)

# Executable
TARGET = test_suite

# Rules
all: $(TARGET)

$(TARGET): $(OBJS) $(TEST_OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^ $(GTEST_DIR)/lib/libgtest.a $(GTEST_DIR)/lib/libgtest_main.a $(LIBS)

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILDDIR)/%.o: $(TESTDIR)/%.cpp
	@mkdir -p $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -isystem $(GTEST_DIR)/include -c $< -o $@

clean:
	rm -rf $(BUILDDIR) $(TARGET) *.gcda *.gcno *.gcov coverage.info

.PHONY: all clean
