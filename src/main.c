#include "my_printf.h"
#include "my_string.h"
int main(void)
{
    MyString greeting = my_new_string("hello world");
    my_printf("%S\n", &greeting);
    my_destroy_string(&greeting);
    return 0;
}
