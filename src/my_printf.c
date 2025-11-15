#include "my_string.h"
#include <stdarg.h>
#include <unistd.h>
/*
 * %d : integer
 * %s : string
 * %c : character
 * %% : the character '%'
 * %S : MyString*
 */

void parse_element(MyString *output, char key, va_list *args)
{
    if (key == 'd')
    {
        int var = va_arg(*args, int);
        MyString var_literal = my_int_to_string(var);
        my_append_string(output, &var_literal);
        my_destroy_string(&var_literal);
    }
    else if (key == 's')
    {
        char *var = va_arg(*args, char *);
        if (var == NULL)
        {
            my_append_literal(output, "(null)");
        }
        else
            my_append_literal(output, var);
    }
    else if (key == 'c')
    {
        int temp = va_arg(*args, int);
        char var = (char)temp;
        my_append_character(output, var);
    }
    else if (key == '%')
    {
        my_append_character(output, '%');
    }
    else if (key == 'S')
    {
        MyString *var = va_arg(*args, MyString *);
        my_append_string(output, var);
    }
    else
    {
        my_append_character(output, '%');
        my_append_character(output, key);
    }
}
int my_printf(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    int format_length = my_calculate_string_length(format);
    MyString output = my_new_string("");

    for (int i = 0; i < format_length; i++)
    {
        char element = format[i];

        if (element != '%')
        {
            my_append_character(&output, element);
        }
        else if (i == format_length - 1 && element == '%')
        {
            my_append_character(&output, '%');
        }
        else
        {
            parse_element(&output, format[i + 1], &args);
            i++;
        }
    }
    int bytes_written = output.size;
    write(1, output.string_proper, bytes_written);
    my_destroy_string(&output);
    va_end(args);
    return bytes_written;
}
