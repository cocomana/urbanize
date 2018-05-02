#include <stdio.h>

#include "../src/list.h"

int main()
{
    ptr_list basic_list = create_list(int);

    list_add(basic_list, 5);

    printf("Value: %d", list_at(basic_list, 0));

    list_destroy(basic_list);

    return 0;
}