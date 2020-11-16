CC = g++

  # compiler flags:
  #   #  -g    adds debugging information to the executable file
    #  -Wall turns on most, but not all, compiler warnings
CFLAGS = -Wl,--no-as-needed -Wno-unused-parameter -ldl -Wall -Wextra -pedantic -g
    #
    #      # the build target executable:
TARGET = test0

all: $(TARGET)

$(TARGET): $(TARGET).cpp
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).cpp

clean:
	$(RM) $(TARGET)