# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -Wextra -Werror
SFMLFLAGS = -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

# Directories
OBJDIR = obj
BINDIR = bin

# Source files
OBJS = $(patsubst %.cpp,$(OBJDIR)/%.o,$(wildcard *.cpp))

# Executable
TARGET = $(BINDIR)/sfml-app

# Rules
all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(BINDIR)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(SFMLFLAGS)

$(OBJDIR)/%.o: %.cpp
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJDIR) $(BINDIR)

.PHONY: all clean
