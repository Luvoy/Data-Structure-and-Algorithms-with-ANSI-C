#include "type.h"

element_type ERROR_ELEMENT = -1;
size_type ERROR_SIZE = -1;

my_bool TRUE = 1;
my_bool FALSE = 0;
int compare(element_type a, element_type b)
{
    if (a > b)
        return 1;
    else if (a == b)
        return 0;
    else
        return -1;
}