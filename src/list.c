#include "list.h"

#include <stdio.h>
#include <string.h>
#include <math.h>

BYTE_ARRAY list_at(_this, int index)
{
    if (index >= 0 && index < this->size) {
        return (*(ptr_value*)(this->_heap_ptr + (this->_item_size * index)));
    } else {
        return 0;
    }
}

ptr_value list_at_ptr(_this, int index)
{
    if (index >= 0 && index < this->size) {
        return _list_index_ptr(this, index);
    } else {
        return 0;
    }
}

ptr_value _list_index_ptr(_this, int index)
{
    return ((ptr_value*)(this->_heap_ptr + (this->_item_size * index)));
}

void list_add_ptr(_this, ptr_value value)
{
    list_add_ptr_at(this, value, this->size);
}

void list_add_ptr_at(_this, ptr_value value, int index)
{
    if (index >= 0 && index <= this->size) {
        this->size++;

        _list_sanitize_heap(this);

        for(int i = this->size - 1; i > index; i--)
        {
            list_set_ptr(this, i, list_at_ptr(this, i - 1));
        }

        list_set_ptr(this, index, value);
    }
}

void list_append_list(_this, ptr_list ptr_add_list)
{
    list_append_list_at(this, ptr_add_list, this->size);
}

void list_append_list_at(_this, ptr_list ptr_add_list, int index)
{
    if (index >= 0 && index <= this->size && this->_item_size == ptr_add_list->_item_size) {
        _list_expand_heap(this, (int)(ptr_add_list->_heap_size/ptr_add_list->_options.heap_block_size));

        ptr_value ptr_index = _list_index_ptr(this, index);
        size_t distance = ptr_add_list->_item_size * ptr_add_list->size;
        size_t size_copy = (this->size - index) * this->_item_size;

        memcpy((ptr_value)(ptr_index + (int)distance), ptr_index, size_copy);
        memcpy(ptr_index, ptr_add_list->_heap_ptr, distance);

        this->size += ptr_add_list->size;

        _list_sanitize_heap(this);
    }
}

void list_remove(_this)
{
    list_remove_at(this, this->size-1);
}

void list_remove_at(_this, int index)
{
    if (index >= 0 && index < this->size) {
        for(int i = index; i < this->size - 1; i++)
        {
            list_set_ptr(this, i, list_at_ptr(this, i + 1));
        }

        this->size--;

        _list_sanitize_heap(this);
    }
}

ptr_list list_sub(_this, int from, int to)
{
    if (from >= 0 && from < this->size && to > from && to < this->size) {
        ptr_list list = create_list_ex(this->_options);
        list->size = (to + 1) - from;

        _list_sanitize_heap(list);

        ptr_value start = _list_index_ptr(this, from);
        size_t size = list->_item_size * list->size;

        memcpy(list->_heap_ptr, start, size);

        return list;
    }
    return 0;
}

ptr_list list_split(_this, int index)
{
    if (index >= 0 && index < this->size) {
        ptr_list list = create_list_ex(this->_options);
        list->size = this->size - index;

        _list_sanitize_heap(list);

        ptr_value start = _list_index_ptr(this, index);
        size_t size = list->_item_size * list->size;

        memcpy(list->_heap_ptr, start, size);

        this->size -= list->size;

        _list_sanitize_heap(this);

        return list;
    }
    return 0;
}

int list_index_of_ptr(_this, ptr_value value)
{
    int index = -1;
    for (int i = 0; i < this->size; i++)
    {
        if ((memcmp(list_at_ptr(this, i), value, this->_item_size) == 0))
        {
            index = i;
            break;
        }
    }

    return index;
}

int list_last_index_of_ptr(_this, ptr_value value)
{
    int index = -1;
    for (int i = this->size - 1; i >= 0; i--)
    {
        if ((memcmp(list_at_ptr(this, i), value, this->_item_size) == 0))
        {
            index = i;
            break;
        }
    }

    return index;
}

void list_set_ptr(_this, int index, ptr_value value)
{
    if (index >= 0 && index < this->size) {
        memcpy(((ptr_value *)(this->_heap_ptr + (this->_item_size * index))), value, this->_item_size);
    }
}

void _list_set_blocks(_this, int blocks)
{
    this->_heap_size = (this->_options.heap_block_size * blocks);
    this->_heap_ptr = realloc(this->_heap_ptr, this->_heap_size * this->_item_size);
}

void _list_expand_heap(_this, int blocks)
{
    this->_heap_size += (this->_options.heap_block_size * blocks);
    this->_heap_ptr = realloc(this->_heap_ptr, this->_heap_size * this->_item_size);
}

void _list_reduce_heap(_this, int blocks)
{
    this->_heap_size -= (this->_options.heap_block_size * blocks);
    this->_heap_ptr = realloc(this->_heap_ptr, this->_heap_size * this->_item_size);
}

void _list_sanitize_heap(_this)
{
    int block_count = (int) this->_heap_size / (int) this->_options.heap_block_size;
    int optimal_block_count = (int)((((int)this->_heap_size - (double)this->size) / (double)this->_options.heap_block_size));

    if ((this->size % 10) < 5) optimal_block_count++;

    if (block_count != optimal_block_count) {
        _list_set_blocks(this, optimal_block_count);
    }
}

void list_foreach(_this, foreach_run run)
{
    for(int i = 0; i < this->size; i++)
    {
        run(this, list_at_ptr(this, i));
    }
}

void list_clear(_this)
{
    this->size = 0;

    _list_set_blocks(this, 1);
}

void list_destroy(_this)
{
    free(this->_heap_ptr);
    free((void*) this);
}

// Creates the default options and creates a new list
ptr_list create_list(size_t item_size)
{
    list_options options;
    options.heap_block_size = 10;
    options.item_size = item_size;

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

    list->size = 0;

    return list;
}
