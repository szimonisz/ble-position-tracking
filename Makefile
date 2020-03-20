CC =gcc # the C compiler

SRCDIR = src
BUILDDIR = build
TARGETDIR = bin
TARGET = $(TARGETDIR)/trilateration-server
 
INC = -I include
DEPS = trilatmath.h
OBJECTS = $(BUILDDIR)/trilateration-server.o $(BUILDDIR)/trilatmath.o

$(TARGET): $(OBJECTS)
        # -o specifies the output filename
	@mkdir -p $(TARGETDIR)
	@echo "$(CC) -o $(TARGET) $(OBJECTS) $(INC)"
	$(CC) -o $(TARGET) $(OBJECTS) $(INC)

$(BUILDDIR)/%.o: $(SRCDIR)/%.c
        # -c: compile the source file into the specified object file
	@mkdir -p $(BUILDDIR)
	@echo "$(CC) -o $@ -c $< $(INC)"
	$(CC) -o $@ -c $< $(INC)

clean:
	rm -r $(BUILDDIR) $(TARGETDIR)
