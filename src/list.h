#ifndef CDYNAMICARRAYS_LIST_H
#define CDYNAMICARRAYS_LIST_H

#define _this ptr_list this

#include <stdlib.h>
#include <stdint.h>
#include "list_options.h"

#if _WIN32 || _WIN64
#if _WIN64
#define PTR_SIZE 8
#else
#define PTR_SIZE 4
#endif
#endif

#if __GNUC__
#if __x86_64__ || __ppc64__
#define PTR_SIZE 8
#else
#define PTR_SIZE 4
#endif
#endif

#define ptr_value void*

#define BYTE unsigned char
#define BYTE_ARRAY BYTE*

#define _S (char*)

typedef struct _list _list, *ptr_list;
struct _list
{
    list_options _options;

    ptr_value _heap_ptr;
    size_t _heap_size;

    int size;

    size_t _item_size;
};

BYTE_ARRAY list_at(_this, int index);
ptr_value list_at_ptr(_this, int index);
ptr_value _list_index_ptr(_this, int index);

void list_add_ptr(_this, ptr_value value);
void list_add_ptr_at(_this, ptr_value value, int index);
#define list_add(list, value) {typeof(value) VAL = value; list_add_ptr(list, &VAL);}
#define list_add_at(list, value, index) {typeof(value) VAL = value; list_add_ptr_at(list, &VAL, index);}

void list_append_list(_this, ptr_list ptr_add_list);
void list_append_list_at(_this, ptr_list ptr_add_list, int index);

void list_set_ptr(_this, int index, ptr_value value);
#define list_set(list, index, value) ({typeof(value) VAL = value; list_set_ptr(list, index, &VAL);})

void list_remove(_this);
void list_remove_at(_this, int index);

ptr_list list_sub(_this, int from, int to);
ptr_list list_split(_this, int index);

int list_index_of_ptr(_this, ptr_value value);
int list_last_index_of_ptr(_this, ptr_value value);
#define list_index_of(list, value) ({typeof(value) VAL = value; int index; index = list_index_of_ptr(list, &VAL); index;})
#define list_last_index_of(list, value) ({typeof(value) VAL = value; int index; index = list_last_index_of_ptr(list, &VAL); index;})

void _list_set_blocks(_this, int blocks);
void _list_expand_heap(_this, int blocks);
void _list_reduce_heap(_this, int blocks);
void _list_sanitize_heap(_this);

typedef void (*foreach_run)(ptr_list list, ptr_value value);
void list_foreach(_this, foreach_run run);

void list_clear(_this);
void list_destroy(_this);

ptr_list create_list(size_t item_size);
ptr_list create_list_ex(list_options options);

#endif //CDYNAMICARRAYS_LIST_H
