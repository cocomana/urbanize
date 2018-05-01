#include "../src/list.h"
#include <stdio.h>

typedef struct _num_pair num_pair, *ptr_num_pair;
struct _num_pair
{
    int x;
    int y;
};

int lcm(int a, int b) {
    int num = ((a>b) ? a : b), ret;
    for(ret = 1; ((ret*num) % ((num == b) ? a : b)) != 0; ret++);
    return ret*num;
}

int gcf(int a, int b) {
    for(int i = ((a<b) ? a : b); i > 0; i--) {
        if(a % i == 0 && b % i == 0) return i;
    }
    return 1;
}

void calculate(ptr_list list, ptr_value value)
{
    ptr_num_pair pair = (ptr_num_pair)value;
    printf("X: %d; Y: %d; LCM: %d; GCF: %d;\n", pair->x, pair->y, lcm(pair->x, pair->y), gcf(pair->x, pair->y));
}

void print_all_strings(ptr_list list, ptr_value value)
{
    char* string = *(char**)value;
    printf("String: %s\n", string);
}

num_pair create_num_pair(int x, int y)
{
    num_pair pair;

    pair.x = x;
    pair.y = y;

    return pair;
}

int main()
{
    // GCF and LCM
    printf("\nGCF and LCM: \n");
    ptr_list list = create_list(sizeof(num_pair));
    ptr_list list2 = create_list(sizeof(num_pair));

    list_add(list, create_num_pair(5,10)); // Removed
    list_add(list, create_num_pair(4,12));

    list_add(list2, create_num_pair(18,111));
    list_add(list2, create_num_pair(100,90));

    list_add_at(list, create_num_pair(23,75), 0); // Set to first element

    list_append_list(list, list2);

    list_remove_at(list, 1);

    list_foreach(list, calculate);

    list_destroy(list);
    list_destroy(list2);

    // Examples of primitives: Strings
    printf("\nStrings:\n");
    list = create_list(sizeof(char*));

    list_add(list, _S"This is a string");
    list_add(list, _S"This is a second string");
    list_add(list, _S"This is a third string");
    list_add(list, _S"This is a fourth string");

    list_foreach(list, print_all_strings);

    ptr_list split = list_split(list, 2);

    printf("\nFirst list:\n");
    list_foreach(list, print_all_strings);

    printf("\nSplit list:\n");
    list_foreach(split, print_all_strings);

    list_clear(list);

    printf("\nFirst list:\n");
    list_foreach(list, print_all_strings);

    printf("\nSplit list:\n");
    list_foreach(split, print_all_strings);

    list_destroy(split);
    list_destroy(list);

    // Examples of primitives: Integers
    printf("\nIntegers:\n");
    list = create_list(sizeof(int));

    list_add(list, 10);
    list_add(list, 55);
    list_add(list, 100);
    list_add(list, 5);

    list_set(list, 1, 16);

    printf("First number: %d\nSecond number: %d\nThird number: %d\nFourth number: %d\n", list_at(list, 0), list_at(list, 1), list_at(list, 2), list_at(list, 3));

    ptr_list sub = list_sub(list, 0, 1);

    printf("First number: %d\nSecond number: %d\n", list_at(sub, 0), list_at(sub, 1));

    printf("Index of 16: %d", list_index_of(list, 16));

    list_destroy(sub);
    list_destroy(list);
}