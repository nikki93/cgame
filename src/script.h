#ifndef SCRIPT_H
#define SCRIPT_H

#include "scalar.h"
#include "script_export.h"
#include "saveload.h"
#include "input.h"

void script_run_string(const char *s);
void script_run_file(const char *filename);

void script_init();
void script_deinit();
void script_clear();
void script_update_all();
void script_draw_all();
void script_key_down(KeyCode key);
void script_save_all(Serializer *s);
void script_load_all(Deserializer *s);

#endif

