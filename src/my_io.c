// #include "my_io.h"
// #include "my_string.h"
// #include <fcntl.h>
// #include <sys/select.h>
// #include <sys/stat.h>
// #include <sys/types.h>
// #include <unistd.h>
//
// int my_open(const char *pathname, char read_write)
// {
//     if (read_write == 'r')
//     {
//         return open(pathname, O_RDONLY, 0666);
//     }
//     if (read_write == 'w')
//     {
//         return open(pathname, O_WRONLY, 0666);
//     }
//     if (read_write == 'o')
//     {
//         return open(pathname, O_RDWR, 0666);
//     }
//     else
//     {
//         return -1;
//     }
// }
//
// int my_read_full(MyString *file_string, int file_d, int count)
// {
//     file_string->size = 0;
//     file_string->capacity = 100;
//     file_string->string_proper = malloc(file_string->capacity);
//     if (file_string->string_proper == NULL)
//     {
//         return -1;
//     }
//
//     char temp[count + 1];
//     ssize_t bytes_read = read(file_d, temp, count);
//     if (bytes_read < 0)
//     {
//         return -1;
//     }
//     temp[bytes_read] = '\0';
//     append_literal(file_string, temp);
//     return 0;
// }
//
// int my_close(int file_d)
// {
//     return close(file_d);
// }
//
// int my_write_file(MyString *string, int file_d)
// {
//     lseek(file_d, 0, SEEK_SET);
//     return write(file_d, string->string_proper, string->size);
// }
//
// int my_write_append(MyString *string, int file_d)
// {
//     lseek(file_d, 0, SEEK_END);
//     return write(file_d, string->string_proper, string->size);
// }
