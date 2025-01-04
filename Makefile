######################################################################
# @author      : mailitg (mailitg@$HOSTNAME)
# @file        : Makefile
# @created     : Wednesday Jan 01, 2025 20:01:44 CET
######################################################################

CXX = g++
CXXFLAGS = -Iinclude -Isrc -std=c++17

OBJDIR = build/obj
LIBDIR = build/lib
LIBNAME = libsimpleecs.a

SRC = $(wildcard src/*.cpp)
OBJ = $(SRC:src/%.cpp=$(OBJDIR)/%.o)

# Example program source 
EXAMPLE_SRC = example/main.cpp
EXAMPLE_OBJ = $(OBJDIR)/main.o
EXAMPLE_BIN = build/bin/test_program

LDFLAGS = -L$(LIBDIR) -lsimpleecs

all: $(LIBDIR)/$(LIBNAME) $(EXAMPLE_BIN)

$(LIBDIR)/$(LIBNAME): $(OBJ)
	mkdir -p $(LIBDIR)
	ar rcs $@ $(OBJ)

$(OBJDIR)/%.o: src/%.cpp
	mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(EXAMPLE_OBJ): $(EXAMPLE_SRC)
	mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(EXAMPLE_BIN): $(EXAMPLE_OBJ) $(LIBDIR)/$(LIBNAME)
	mkdir -p build/bin
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDFLAGS)

clean:
	rm -rf build

run: 
	./$(EXAMPLE_BIN)
