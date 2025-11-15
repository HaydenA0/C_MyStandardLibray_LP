#ifndef MYIO_H
#define MYIO_H
#include "my_string.h"
#include <stddef.h>
#define BUFFER_SIZE 1024
/*
 * Mode that the functions take are
 * "r" : reading, file must exist
 * "w" : write , overwrite if exists, create if not existing
 * "a" : append, write at the end, create is not existing
 * "r+" : read/write : file must exist
 * "w+" : read/write : overwrite if exists, create if not existing
 * "a+" : read/write : write at the end, create if not existing
 */
typedef struct MyFile
{
    int fd;                   // file descriptor
    char buffer[BUFFER_SIZE]; // change the buffer_size if u change this
    int mode;                 // read or write
    size_t buffer_current_pos;
    size_t buffer_end_pos;
    int eof;
    int error;
} MyFile;

MyFile *myfopen(const char *filepath, const char *mode);
MyString myfread(MyFile *file, size_t nbytes);

#endif
