CC?=clang
LINKER=clang
CFLAGS?=-g -I. -I/usr/local/include/SDL2 -DGLEW_STATIC
LDFLAGS?=-g -framework OpenGL -lglfw3 -lGLEW -lfreeimage -lluajit-5.1 \
	 -DGLEW_STATIC -pagezero_size 10000 -image_base 100000000
SOURCES=main.c vec2.c game.c sprite.c transform.c entity.c system.c \
	test/keyboard_controlled.c test/test.c

OBJS=$(SOURCES:.c=.o)

all: cgame cgame_ffi.h

clean:
	rm -rf cgame cgame_ffi.h $(OBJS)

cgame: $(OBJS)
	$(LINKER) $(LDFLAGS) -o cgame $(OBJS) 

cgame_ffi.h: game.h entity.h transform.h sprite.h test/keyboard_controlled.h \
    vec2.h
	lua gen_cgame_ffi.lua

.c.o:
	$(CC) $(CFLAGS) -o $(<:.c=.o) -c $<


