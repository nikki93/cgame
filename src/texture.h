#ifndef TEXTURE_H
#define TEXTURE_H

void texture_load(const char *filename);
void texture_bind(const char *filename);
int texture_get_width(const char *filename);
int texture_get_height(const char *filename);

void texture_init();
void texture_deinit();

#endif

