#include "my_file_io.h"
#include "my_string.h"
#include <fcntl.h>
#include <stddef.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

MyFile *myfopen(const char *filepath, const char *mode)
{
    MyFile *opened_file = malloc(sizeof(MyFile));
    int flags;
    if (my_compare_literals(mode, "r"))
        flags = O_RDONLY;
    else if (my_compare_literals(mode, "w"))
        flags = O_WRONLY | O_CREAT | O_TRUNC;
    else if (my_compare_literals(mode, "a"))
        flags = O_WRONLY | O_CREAT | O_APPEND;
    else if (my_compare_literals(mode, "r+"))
        flags = O_RDWR;
    else if (my_compare_literals(mode, "w+"))
        flags = O_RDWR | O_CREAT | O_TRUNC;
    else if (my_compare_literals(mode, "a+"))
        flags = O_RDWR | O_CREAT | O_APPEND;
    else
    {
        return NULL;
    }
    opened_file->fd = open(filepath, flags, 0644);
    if (opened_file->fd < 0)
    {
        return NULL;
    }
    opened_file->buffer_current_pos = 0;
    opened_file->buffer_end_pos = 0;
    opened_file->eof = 0;
    opened_file->error = 0;
    return opened_file;
}
MyString myfread(MyFile *file, size_t nbytes)
{
    MyString data_read = my_new_string("");
    size_t remaining = nbytes;
    while (remaining > 0)
    {
        size_t to_read = remaining;
        if (to_read > BUFFER_SIZE)
            to_read = BUFFER_SIZE;
        size_t bytes_read = read(file->fd, file->buffer, to_read);
        if (bytes_read <= 0)
            break;
        my_append_custom_literal(&data_read, file->buffer, bytes_read);
        remaining -= bytes_read;
        // debug
    }
    return data_read;
}
