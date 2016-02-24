CFLAGS =
# INCS =
TARGET = bin/texitree
SOURCES = texitree.c
# LIBS =  ``
CC = gcc-5.3.0


all: $(TARGET)

$(TARGET): $(SOURCES) $(INCS)
	$(CC) $(SOURCES) -o $(TARGET) $(CFLAGS) $(LIBS)

clean:
	rm -f $(TARGET)
