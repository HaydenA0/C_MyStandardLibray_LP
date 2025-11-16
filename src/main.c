#include "my_allocater.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main()
{
    int size = 10240;
    int *ptr = my_allocate(size);
    for (int i = 0; i < size; i++)
    {
        ptr[i] = i;
        printf("ptr[%d] = %d\n", i, ptr[i]);
    }
    my_free(ptr);
    return 0;
}
