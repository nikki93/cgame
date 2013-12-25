#ifndef ENTITY_H_GUOK04WE
#define ENTITY_H_GUOK04WE

#include <stdlib.h>

#define ENTITY_MAX 256

/* script_begin */

typedef unsigned int Entity;

Entity entity_new();

/* script_end */

void entity_deinit();
void entity_update_all();

/* ------------------------------------------------------------------------- */

enum
{
    MSG_DESTROY
};

typedef struct Message Message;

void *entity_new_message(Entity ent, unsigned int message_type,
        size_t data_size);
Message *entity_get_first_message(Entity ent);
Message *entity_get_next_message(Message* msg);
unsigned int message_get_type(Message *msg);
void *message_get_data(Message *msg);

#endif


