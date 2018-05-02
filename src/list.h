#ifndef CDYNAMICARRAYS_LIST_H
#define CDYNAMICARRAYS_LIST_H

#include <stdlib.h>
#include <stdint.h>
#include "list_options.h"

typedef struct _list _list, *ptr_list;
struct _list
{
    list_options _options;

    void* _heap_ptr;
    size_t _heap_size;

    int size;

    size_t _item_size;
};

void* list_at_ptr(ptr_list this_list, int index);
#define list_at(list, index, type)({*(type*)list_at_ptr(list, index);})

void list_add_ptr(ptr_list this_list, void* value);
void list_add_ptr_at(ptr_list this_list, void* value, int index);
#define list_add(list, value) {typeof(value) VAL = value; list_add_ptr(list, &VAL);}
#define list_add_at(list, value, index) {typeof(value) VAL = value; list_add_ptr_at(list, &VAL, index);}

void list_append_list(ptr_list this_list, ptr_list ptr_add_list);
void list_append_list_at(ptr_list this_list, ptr_list ptr_add_list, int index);

void list_set_ptr(ptr_list this_list, int index, void* value);
#define list_set(list, index, value) ({typeof(value) VAL = value; list_set_ptr(list, index, &VAL);})

void list_remove(ptr_list this_list);
void list_remove_at(ptr_list this_list, int index);

ptr_list list_sub(ptr_list this_list, int from, int to);
ptr_list list_split(ptr_list this_list, int index);

int list_index_of_ptr(ptr_list this_list, void* value);
int list_last_index_of_ptr(ptr_list this_list, void* value);
#define list_index_of(list, value) ({typeof(value) VAL = value; int index; index = list_index_of_ptr(list, &VAL); index;})
#define list_last_index_of(list, value) ({typeof(value) VAL = value; int index; index = list_last_index_of_ptr(list, &VAL); index;})

typedef void (*foreach_run)(ptr_list list, void* value);
void list_foreach(ptr_list this_list, foreach_run run);

void list_clear(ptr_list this_list);
void list_destroy(ptr_list this_list);

ptr_list create_list_size(size_t item_size, int length);
ptr_list create_list_ex(list_options options);

#define create_list(type)({create_list_size(sizeof(type), 0);})
#define create_list_length(type, length)({create_list_size(sizeof(type), length);})

#endif //CDYNAMICARRAYS_LIST_H
