#include "my_printf.h"
int main(void)
{
    int age = 19;
    int siblings_number = 3;
    char *love_to_hajar = "infinitely";
    my_printf(
        "I am %d old.\nI have %d siblings.\nAnd I love them but not as much as I love my girl.\nI love here %s.\n", age,
        siblings_number, love_to_hajar);

    return 0;
}
