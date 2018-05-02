#include <stdio.h>

#include "../src/list.h"

typedef struct mystruct
{
    int x;
    int y;
};

mystruct create_mystruct(int x, int y)
{
    mystruct mystr;

    mystr.x = x;
    mystr.y = y;

    return mystr;
}

int main()
{
    ptr_list custom_list = create_list(mystruct);

    list_add(custom_list, create_mystruct(1,2));

    printf("Value: %d", (list_at(custom_list, 0)).x);

    list_destroy(custom_list);

    return 0;
}