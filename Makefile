TARGET = logic
CC     = gcc
OBJS   = expression.o environment.o evaluate.o reader.o main.o
CFLAGS = -Wall -Werror -ggdb

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

.SUFFIXES: .c .0
.c.o:
	$(CC) $(CFLAGS) -c $<

clean:
	rm $(OBJS) $(TARGET)

# flymake-mode
# ---
check-syntax:
	$(CC) -o nul -S ${CHK_SOURCES}
