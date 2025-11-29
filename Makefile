# Makefile for MediaShop C++ Course Work
# Builds the console application for media file buying/selling system

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic
TARGET = mediashop
SRCS = main.cpp Auth.cpp User.cpp MediaFile.cpp MediaShop.cpp Menu.cpp
OBJS = $(SRCS:.cpp=.o)

# Default target
all: $(TARGET)

# Link object files
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

# Compile source files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -f $(OBJS) $(TARGET)

# Clean all generated files
distclean: clean
	rm -f shop.txt users.txt login.log *.txt

# Run the application
run: $(TARGET)
	./$(TARGET)

# Dependencies
main.o: main.cpp Auth.h User.h MediaFile.h MediaShop.h
Auth.o: Auth.cpp Auth.h
User.o: User.cpp User.h MediaFile.h
MediaFile.o: MediaFile.cpp MediaFile.h
MediaShop.o: MediaShop.cpp MediaShop.h User.h MediaFile.h
Menu.o: Menu.cpp User.h MediaShop.h MediaFile.h

.PHONY: all clean distclean run
