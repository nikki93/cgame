CC?=clang
LINKER=clang
CFLAGS?=-g -I. -I/usr/local/include/SDL2 -DGLEW_STATIC
LDFLAGS?=-g -framework OpenGL -lglfw3 -lGLEW -lfreeimage -lluajit-5.1 \
	 -DGLEW_STATIC -pagezero_size 10000 -image_base 100000000
SOURCES=main.c vec2.c mat3.c game.c entity.c system.c script.c transform.c \
	camera.c sprite.c test/keyboard_controlled.c test/test.c
FFI=game.h vec2.h mat3.h entity.h transform.h camera.h sprite.h \
    test/keyboard_controlled.h


OBJS=$(SOURCES:.c=.o)

all: cgame cgame_ffi.h

clean:
	rm -rf cgame cgame_ffi.h $(OBJS)

cgame: $(OBJS)
	$(LINKER) $(LDFLAGS) -o cgame $(OBJS) 

cgame_ffi.h: $(FFI)
	python gen_cgame_ffi.py $(FFI)

script.o: cgame_ffi.h

.c.o:
	$(CC) $(CFLAGS) -o $(<:.c=.o) -c $<


