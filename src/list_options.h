#ifndef CDYNAMICARRAYS_TEST_LIST_OPTIONS_H
#define CDYNAMICARRAYS_TEST_LIST_OPTIONS_H

#include <stdlib.h>

typedef struct list_options list_options, *ptr_list_options;
struct list_options
{
    size_t heap_block_size;
    size_t item_size;

    int size_start;
};

#endif //CDYNAMICARRAYS_TEST_LIST_OPTIONS_H
