CC = g++
CFLAGS = -g

TARGET = main

# Dir's for header .cpp and obj files
IDIR=include
SDIR=src
ODIR=$(SDIR)/obj

SRCS=$(wildcard $(SDIR)/*.cpp) # Identify all source files
DEPS=$(wildcard $(IDIR)/*.hpp) # Identify all include files
OBJ=$(patsubst $(SDIR)/%.cpp,$(ODIR)/%.o,$(SRCS)) # Indentify object file names

# Rule to make all .cpp files into object files
# -c tells the compiler to create obj file.
# If any header file is changed then all 
# files all compiled again
$(ODIR)/%.o: $(SDIR)/%.cpp $(DEPS)
	@$(CC) $< -c -o $@ $(CFLAGS)

# Rule to make obj files into exectuable
make: $(OBJ)
	@$(CC) -o $(TARGET) $(OBJ) $(CFLAGS)


.PHONY: print clean cleanall
print:
	@echo $(SRCS)
	@echo $(OBJ)
	@echo $(DEPS)

clean:
	@rm -f $(ODIR)/*.o

cleanall:
	@rm -f $(TARGET) $(ODIR)/*.o
