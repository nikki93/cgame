#ifndef TEXTURE_H_QIELURFK
#define TEXTURE_H_QIELURFK

void texture_load(const char *filename);
void texture_bind(const char *filename);
int texture_get_width(const char *filename);
int texture_get_height(const char *filename);

void texture_init();
void texture_deinit();

#endif

