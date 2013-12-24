CC?=clang
LINKER=clang
CFLAGS?=-g -I. -I/usr/local/include/SDL2 -DGLEW_STATIC
LDFLAGS?=-g -framework OpenGL -lglfw3 -lGLEW -lfreeimage -lluajit-5.1 \
	 -DGLEW_STATIC -pagezero_size 10000 -image_base 100000000
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


