#include "list.h"

#include <stdio.h>
#include <string.h>
#include <math.h>

void* _list_index_ptr(ptr_list this_list, int index);

void _list_set_blocks(ptr_list this_list, int blocks);
void _list_expand_heap(ptr_list this_list, int blocks);
void _list_reduce_heap(ptr_list this_list, int blocks);
void _list_sanitize_heap(ptr_list this_list);

void _heap_realloc(ptr_list this_list);

void* list_at_ptr(ptr_list this_list, int index)
{
    if (index >= 0 && index < this_list->size) {
        return _list_index_ptr(this_list, index);
    } else {
        return 0;
    }
}

void* _list_index_ptr(ptr_list this_list, int index)
{
    return ((void**)(this_list->_heap_ptr + (this_list->_item_size * index)));
}

void list_add_ptr(ptr_list this_list, void* value)
{
    list_add_ptr_at(this_list, value, this_list->size);
}

void list_add_ptr_at(ptr_list this_list, void* value, int index)
{
    if (index >= 0 && index <= this_list->size) {
        this_list->size++;

        _list_sanitize_heap(this_list);

        for(int i = this_list->size - 1; i > index; i--)
        {
            list_set_ptr(this_list, i, list_at_ptr(this_list, i - 1));
        }

        list_set_ptr(this_list, index, value);
    }
}

void list_append_list(ptr_list this_list, ptr_list ptr_add_list)
{
    list_append_list_at(this_list, ptr_add_list, this_list->size);
}

void list_append_list_at(ptr_list this_list, ptr_list ptr_add_list, int index)
{
    if (index >= 0 && index <= this_list->size && this_list->_item_size == ptr_add_list->_item_size) {
        _list_expand_heap(this_list, (int)(ptr_add_list->_heap_size/ptr_add_list->_options.heap_block_size));

        void* ptr_index = _list_index_ptr(this_list, index);
        size_t distance = ptr_add_list->_item_size * ptr_add_list->size;
        size_t size_copy = (this_list->size - index) * this_list->_item_size;

        memcpy((void*)(ptr_index + (int)distance), ptr_index, size_copy);
        memcpy(ptr_index, ptr_add_list->_heap_ptr, distance);

        this_list->size += ptr_add_list->size;

        _list_sanitize_heap(this_list);
    }
}

void list_remove(ptr_list this_list)
{
    list_remove_at(this_list, this_list->size-1);
}

void list_remove_at(ptr_list this_list, int index)
{
    if (index >= 0 && index < this_list->size) {
        for(int i = index; i < this_list->size - 1; i++)
        {
            list_set_ptr(this_list, i, list_at_ptr(this_list, i + 1));
        }

        this_list->size--;

        _list_sanitize_heap(this_list);
    }
}

ptr_list list_sub(ptr_list this_list, int from, int to)
{
    if (from >= 0 && from < this_list->size && to > from && to < this_list->size) {
        ptr_list list = create_list_ex(this_list->_options);
        list->size = (to + 1) - from;

        _list_sanitize_heap(list);

        void* start = _list_index_ptr(this_list, from);
        size_t size = list->_item_size * list->size;

        memcpy(list->_heap_ptr, start, size);

        return list;
    }
    return 0;
}

ptr_list list_split(ptr_list this_list, int index)
{
    if (index >= 0 && index < this_list->size) {
        ptr_list list = create_list_ex(this_list->_options);
        list->size = this_list->size - index;

        _list_sanitize_heap(list);

        void* start = _list_index_ptr(this_list, index);
        size_t size = list->_item_size * list->size;

        memcpy(list->_heap_ptr, start, size);

        this_list->size -= list->size;

        _list_sanitize_heap(this_list);

        return list;
    }
    return 0;
}

int list_index_of_ptr(ptr_list this_list, void* value)
{
    int index = -1;
    for (int i = 0; i < this_list->size; i++)
    {
        if ((memcmp(list_at_ptr(this_list, i), value, this_list->_item_size) == 0))
        {
            index = i;
            break;
        }
    }

    return index;
}

int list_last_index_of_ptr(ptr_list this_list, void* value)
{
    int index = -1;
    for (int i = this_list->size - 1; i >= 0; i--)
    {
        if ((memcmp(list_at_ptr(this_list, i), value, this_list->_item_size) == 0))
        {
            index = i;
            break;
        }
    }

    return index;
}

void list_set_ptr(ptr_list this_list, int index, void* value)
{
    if (index >= 0 && index < this_list->size) {
        memcpy(((void* *)(this_list->_heap_ptr + (this_list->_item_size * index))), value, this_list->_item_size);
    }
}

void _heap_realloc(ptr_list this_list)
{
    void* tmp = realloc(this_list->_heap_ptr, this_list->_heap_size * this_list->_item_size);
    if (tmp) {
        this_list->_heap_ptr = tmp;
    } else {
        printf("ERR: list could not be reallocated!");
    }
}

void _list_set_blocks(ptr_list this_list, int blocks)
{
    this_list->_heap_size = (this_list->_options.heap_block_size * blocks);
    _heap_realloc(this_list);
}

void _list_expand_heap(ptr_list this_list, int blocks)
{
    this_list->_heap_size += (this_list->_options.heap_block_size * blocks);
    _heap_realloc(this_list);
}

void _list_reduce_heap(ptr_list this_list, int blocks)
{
    this_list->_heap_size -= (this_list->_options.heap_block_size * blocks);
    _heap_realloc(this_list);
}

void _list_sanitize_heap(ptr_list this_list)
{
    int block_count = (int) this_list->_heap_size / (int) this_list->_options.heap_block_size;
    int optimal_block_count = (int)((((int)this_list->_heap_size - (double)this_list->size) / (double)this_list->_options.heap_block_size));

    if ((this_list->size % 10) < 5) optimal_block_count++;

    if (block_count != optimal_block_count) {
        _list_set_blocks(this_list, optimal_block_count);
    }
}

void list_foreach(ptr_list this_list, foreach_run run)
{
    for(int i = 0; i < this_list->size; i++)
    {
        run(this_list, list_at_ptr(this_list, i));
    }
}

void list_clear(ptr_list this_list)
{
    this_list->size = 0;

    _list_set_blocks(this_list, 1);
}

void list_destroy(ptr_list this_list)
{
    free(this_list->_heap_ptr);
    free((void*) this_list);
}

// Creates the default options and creates a new list
ptr_list create_list_size(size_t item_size, int length)
{
    list_options options;
    options.heap_block_size = 10;
    options.item_size = item_size;
    options.size_start = length;

    return create_list_ex(options);
}

// Creates a list with the given options
ptr_list create_list_ex(list_options options)
{
    ptr_list list = (ptr_list) malloc(sizeof(_list));

    list->_options = options;
    list->_item_size = options.item_size;

    list->_heap_ptr = malloc(list->_options.heap_block_size * list->_item_size);
    list->_heap_size = list->_options.heap_block_size;

    list->size = options.size_start;

    _list_sanitize_heap(list);

    return list;
}
