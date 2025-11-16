#include "my_allocater.h"
#include "my_file_io.h"
#include "my_printf.h"
#include "my_string.h"
#include <stdio.h>
#include <unistd.h>

int main()
{

    char *mem1 = (char *)my_allocate(32);
    char *mem2 = (char *)my_allocate(64);
    if (mem1 && mem2)
    {
        sprintf(mem1, "Allocator demo");
        sprintf(mem2, "Second block");
        my_printf("mem1: %s\nmem2: %s\n", mem1, mem2);
    }
    my_free(mem1);
    my_free(mem2);

    MyString s1 = my_new_string("Hello");
    MyString s2 = my_new_string("World");
    MyString s3 = my_copy_string(&s1);
    my_append_literal(&s1, ", ");
    my_append_string(&s1, &s2);
    my_insert_char(&s1, '!', s1.size);
    my_printf("Combined string: %S\n", &s1);

    my_replace_char(&s1, 'o', '0');
    my_printf("Replaced 'o'->'0': %S\n", &s1);

    my_reverse_string(&s1);
    my_printf("Reversed: %S\n", &s1);

    my_make_uppercase(&s1);
    my_printf("Uppercase: %S\n", &s1);

    my_make_lowercase(&s1);
    my_printf("Lowercase: %S\n", &s1);

    int pos = my_find_first_occ_character(&s1, 'l');
    my_printf("First 'l' position: %d\n", pos);

    int count = my_count_character(&s1, 'l');
    my_printf("Count of 'l': %d\n", count);

    MyString slice = my_slicing(&s1, 2, 7);
    my_printf("Slice (2-7): %S\n", &slice);

    MyString int_str = my_int_to_string(-12345);
    my_printf("Integer to string: %S\n", &int_str);

    my_destroy_string(&s1);
    my_destroy_string(&s2);
    my_destroy_string(&s3);
    my_destroy_string(&slice);
    my_destroy_string(&int_str);

    MyString custom = my_allocate_custom_size(5, 20);
    my_append_literal(&custom, "abc");
    my_printf("Custom-sized string: %S\n", &custom);
    my_destroy_string(&custom);

    MyFile *file = myfopen("example.txt", "w+");
    if (file)
    {
        write(file->fd, "File I/O demo\n", 14);
        lseek(file->fd, 0, SEEK_SET);
        MyString file_content = myfread(file, 14);
        my_printf("File read: %S\n", &file_content);
        my_destroy_string(&file_content);
        close(file->fd);
        free(file);
    }

    my_printf("Formatted output: %d, %s, %c, %%\n", 100, "hello", 'X');

    void *zero_alloc = my_allocate(0);
    my_printf("Zero-byte allocation returned: %p\n", zero_alloc);
    my_free(zero_alloc);

    MyString empty = my_new_string("");
    my_printf("Empty string: '%S'\n", &empty);
    my_destroy_string(&empty);

    return 0;
}
