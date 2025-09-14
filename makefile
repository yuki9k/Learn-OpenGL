CC      = clang
CFLAGS  = -Wall -Iinclude
LDFLAGS =
LDLIBS  = -lglfw -lm
SRCS    = src/main.c src/glad.c src/shader.c 
OBJS    = $(SRCS:.c=.o)
TARGET  = bin/learnopengl

all: $(TARGET)

$(TARGET): $(OBJS) | bin
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(OBJS) $(LDLIBS)

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

bin:
	mkdir -p bin

clean:
	rm -f $(OBJS) $(TARGET)
