
#include "my_string.h"
#include <stdio.h>

int main(void)
{
    MyString text = my_new_string("abcefo_efa_efb");
    int counter = my_count_character(&text, 'e');
    printf("e was %d times in text\n", counter);

    my_destroy_string(&text);
}
