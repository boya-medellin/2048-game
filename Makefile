CC = gcc
CFLAGS = -lstdc++
TARGET = game

all: $(TARGET)

$(TARGET): $(TARGET).cpp
	$(CC) $(CFLAGS) $(TARGET).cpp -o $(TARGET)

clean: 
	$(RM) $(TARGET)
