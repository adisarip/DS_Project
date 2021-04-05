###############################################################################
# File        : Makefile                                                      #
# Author      : Aditya Saripalli                                              #
# Description : Makefile for building binaries for DS Course Project          #
###############################################################################

TARGET  = sttd
CC      = g++
CCFLAGS = -Wall -std=c++1z
SRCDIR  = ./src
OBJDIR  = ./obj
BINDIR  = ./bin

FILES   := $(wildcard $(SRCDIR)/*.cpp)
OBJECTS := $(FILES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)

$(BINDIR)/$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@
	@echo "Linking Complete."
	@echo "To start the termination detection run --> "$(BINDIR)"/"$(TARGET)

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.cpp
	$(CC) $(CCFLAGS) -c $< -o $@
	@echo "Compiled "$<" successfully."

clean:
	@echo "Cleaning all the object files and binaries."
	rm -f core $(OBJECTS) $(BINDIR)/$(TARGET)
