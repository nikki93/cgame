CC?=clang
LINKER=clang
CFLAGS?=-g -I. -I/usr/local/include/SDL2 -DGLEW_STATIC
LDFLAGS?=-g -framework OpenGL -lglfw3 -lGLEW -lfreeimage -DGLEW_STATIC
SOURCES=main.c game.c sprite.c transform.c entity.c system.c \
	test/keyboard_controlled.c test/test.c

all: cgame

OBJS=$(SOURCES:.c=.o)

clean:
	rm -rf cgame $(OBJS)

cgame: $(OBJS)
	$(LINKER) $(LDFLAGS) -o cgame $(OBJS) 

.c.o:
	$(CC) $(CFLAGS) -o $(<:.c=.o) -c $<


