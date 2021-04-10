###############################################################################
# File        : Makefile                                                      #
# Author      : Aditya Saripalli                                              #
# Description : Makefile for building binaries for DS Course Project          #
###############################################################################

TARGET  = sttd
CC      = mpic++
CCFLAGS = -Wall -std=c++1z
SRCDIR  = ./src
OBJDIR  = ./obj
BINDIR  = ./bin

FILES   := $(wildcard $(SRCDIR)/*.cpp)
OBJECTS := $(FILES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)

$(BINDIR)/$(TARGET): $(OBJECTS)
	@$(CC) $(OBJECTS) -o $@
	@echo "[MAKE] Linking Complete."

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.cpp
	@$(CC) $(CCFLAGS) -c $< -o $@
	@echo "[MAKE] Compiled "$<" successfully."

clean:
	@echo "[MAKE] Cleaning all the object files and binaries."
	@rm -f core $(OBJECTS) $(BINDIR)/$(TARGET)
