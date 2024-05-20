CXX = g++
CXXFLAGS = -std=c++17 -Wall -I/usr/local/include -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lgcov --coverage

# Directories
SRCDIR = ..
BUILDDIR = build
TESTDIR = .

# Source files
SRCS = $(filter-out $(SRCDIR)/main.cpp, $(wildcard $(SRCDIR)/*.cpp))
TEST_SRCS = $(wildcard $(TESTDIR)/*.cpp)
OBJS = $(SRCS:$(SRCDIR)/%.cpp=$(BUILDDIR)/%.o)
TEST_OBJS = $(TEST_SRCS:$(TESTDIR)/%.cpp=$(BUILDDIR)/%.o)

# Libraries
LIBS = -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lgtest -lgtest_main 
GTEST_LIBS = /usr/local/lib/libgtest.a /usr/local/lib/libgtest_main.a
GTEST_FLAGS = -pthread -lgcov --coverage

# Executable
TARGET = test_suite

# Rules
all: $(TARGET)

$(TARGET): $(OBJS) $(TEST_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(GTEST_LIBS) $(LIBS) $(GTEST_FLAGS)

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILDDIR)/%.o: $(TESTDIR)/%.cpp
	@mkdir -p $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILDDIR) $(TARGET)

.PHONY: all clean

