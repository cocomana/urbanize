<h1 align="center">
   <img alt="Logo" src="urbanize.png">
</h1>

<h3 align="center">
   Fast, dynamically allocated, type independent arrays in C.
</h3>

<p align="center">
    <a href="https://choosealicense.com/licenses/gpl-3.0/"><img
		alt="License MIT"
		src="https://img.shields.io/badge/licence-MIT-blue.svg"></a>
    <img alt="German Egineering"
		src="https://img.shields.io/badge/Made%20in-Germany-41af5c.svg">
    <a href="https://cmake.org/"><img 
	    alt="Build with"
		src="https://img.shields.io/badge/Build%20with-CMake-a26d5e.svg">
    </a>
</p>

## Urbanize
Urbanize is a library for creating dynamically allocated, type independent arrays in C. It works for primitive types, literals, and user-defined structures/psuedo classes.

## Basic Example

```C
#include <stdio.h>

#include "../src/list.h"

int main()
{
    ptr_list basic_list = create_list(int);

    list_add(basic_list, 5);

    printf("Value: %d", list_at(basic_list, 0, int));

    list_destroy(basic_list);

    return 0;
}
```

## How to use

Thourough documentation to every function can be found in the section `Exports`. For full examples see `test/list_tests.c`.

### Creation
Simply use the built-in function to create a `list`

```C
ptr_list list = create_list($TYPE);
```

`$TYPE` can be replaced with any primitive type or custom defined type.

### Adding items to the list
Here is where the real magic is done. No matter the type, use `list_add()` to add an item to your list. Both literals and variables can be used here.

```C
ptr_list custom_list = create_list(mystruct);

list_add(custom_list, create_mystruct(1,2));

printf("Value: %d", (list_at(custom_list, 0, mystruct)).x);

list_destroy(custom_list);
```

Note that it is important to call `list_destroy()` when done using a list.

When adding adding literals to a `char*` list, it is important to cast `char*` before the literal, or use the predefined `_S` macro.

### Reading values
`list_at()` allows you access any item in an array at a given index

```C
int item = list_at(list, 0, int);
```

### Looping

You can easily loop through lists using the predefined `list_foreach()` function.

```C
ptr_list list = create_list(int);

list_foreach(list, func);

void func(ptr_list list, void* value)
{
    printf("Value: %d", *(int*) value);
}
```

## Exports

* `list_at(ptr_list list, int index, type)` - Get a value from a list at the given `index`
* `list_at_ptr(ptr_list list, int index)` - Get a pointer to the value at the given `index`
* `list_add_ptr(ptr_list list, ptr_value value)` - Add item to a list from a pointer (value will be copied)
* `list_add_ptr_at(ptr_list list, ptr_value value, int index)` - Add item to a list from a pointer to the given `index` (value will be copied)
* `list_add(ptr_list list, value)` - Add item to the end of a list
* `list_add_at(list, value, index)` - Add item to a list at a given `index`
* `list_append_list(ptr_list list, ptr_list ptr_add_list)` - Append list to another
* `list_append_list_at(ptr_list list, ptr_list ptr_add_list, int index)` - Append list to another at a specific `index`
* `list_set_ptr(ptr_list list, int index, ptr_value value)` - Sets the value of a given `index` from a pointer (value will be copied)
* `list_set(list, index, value)` - Sets the value of a given `index`
* `list_remove(ptr_list list)` - Removes the last item of a list
* `list_remove_at(ptr_list list, int index)` - Removes an item from the list at `index`
* `list_sub(ptr_list list, int from, int to)` - Creates a list from the values between `from` and `to`
* `list_split(ptr_list list, int index)` - Splits a list into two (destructive)
* `list_index_of_ptr(ptr_list list, ptr_value value)` - Returns the index of a pointer in a list
* `list_last_index_of_ptr(ptr_list list, ptr_value value)` - Returns the last index of a pointer in a list
* `list_index_of(list, value)` - Returns the index of a value in a list
* `list_last_index_of(list, value)` - Returns the last index of a value in a list
* `list_foreach(ptr_list list, foreach_run run)` - Runs a given function for all of the items in a list
* `list_clear(ptr_list list)` - Clears all of the items in a list
* `list_destroy(ptr_list list)` - **Important: this must be called when you are done using a list, ** deallocates the list and its heap
* `create_list(type)` - Creates a standard list
* `create_list_size(size_t item_size)` - Creates a standard list
* `create_list_ex(list_options options)` - Creates a list with the given options

## Build

Urbanize uses `CMake` to build. Simply do the following:

`mkdir build && cd build`

`cmake .`

`make`

## Credits

* [Simonwep](https://github.com/Simonwep) - For the logo. Check out [SelectionJS](https://github.com/Simonwep/selection).
