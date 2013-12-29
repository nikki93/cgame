CC?=clang
LINKER=clang
CFLAGS?=-g -I.
LDFLAGS?=-g -framework OpenGL -lglfw3 -lGLEW -lfreeimage -lluajit-5.1 \
	 -pagezero_size 10000 -image_base 100000000
SOURCES=main.c vec2.c mat3.c game.c entity.c system.c saveload.c script.c \
	transform.c camera.c sprite.c test/keyboard_controlled.c test/test.c


OBJS=$(SOURCES:.c=.o)

all: cgame

clean:
	rm -rf cgame $(OBJS)

cgame: $(OBJS)
	$(LINKER) $(LDFLAGS) -o cgame $(OBJS) 

.c.o:
	$(CC) $(CFLAGS) -o $(<:.c=.o) -c $<


