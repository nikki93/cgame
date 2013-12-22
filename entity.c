#include "entity.h"

#include <assert.h>
#include <stdlib.h>

static unsigned int top = 0;

struct Message
{
    unsigned int message_type;
    Message *next;
    char data[0];
};

static Message *messages[ENTITY_MAX];

Entity entity_new()
{
    assert(top < ENTITY_MAX);
    messages[top] = NULL;
    return top++;
}

static void _entity_clear_messages()
{
    Message *msg, *next;
    Entity ent;

    for (ent = 0; ent < top; ++ent)
    {
        for (msg = entity_get_first_message(ent); msg; msg = next)
        {
            next = entity_get_next_message(msg);
            free(msg);
        }

        messages[ent] = NULL;
    }
}

void entity_deinit()
{
    _entity_clear_messages();
}

void entity_update_all()
{
    /* TODO: delete entities with MSG_DESTROY */

    _entity_clear_messages();
}

/* get size_t offset of 'field' in struct 'type' */
#undef offsetof
#define offsetof(type, field) \
    ((size_t) (&((type *) 0)->field))

void *entity_new_message(Entity ent, unsigned int message_type,
        size_t data_size)
{
    Message *msg = (Message *) malloc(offsetof(Message, data) + data_size);
    msg->message_type = message_type;

    msg->next = messages[ent];
    messages[ent] = msg;

    return message_get_data(msg);
}

Message *entity_get_first_message(Entity ent)
{
    return messages[ent];
}

Message *entity_get_next_message(Message* msg)
{
    return msg->next;
}

unsigned int message_get_type(Message *msg)
{
    return msg->message_type;
}

void *message_get_data(Message *msg)
{
    return (void *) &(msg->data);
}

