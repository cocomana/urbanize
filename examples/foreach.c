#include <stdio.h>

#include "../src/list.h"

void print_strings(ptr_list list, void* value)
{
    printf("%s\n", *(char**)value);
}

int main()
{
    ptr_list basic_list = create_list(char*);

    list_add(basic_list, (char*)"This is a string");

    list_destroy(basic_list);

    return 0;
}