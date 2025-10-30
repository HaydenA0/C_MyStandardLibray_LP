// test_my_io.c
#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../my_io.h"
#include "../my_string.h"

// A simple macro for printing test results
#define RUN_TEST(test)                                                                                                 \
    do                                                                                                                 \
    {                                                                                                                  \
        printf("Running %s... ", #test);                                                                               \
        if (test())                                                                                                    \
        {                                                                                                              \
            printf("PASS\n");                                                                                          \
            tests_passed++;                                                                                            \
        }                                                                                                              \
        else                                                                                                           \
        {                                                                                                              \
            printf("FAIL\n");                                                                                          \
            tests_failed++;                                                                                            \
        }                                                                                                              \
    } while (0)

int tests_passed = 0;
int tests_failed = 0;

// Helper function to create a temporary file with specific content
void create_test_file(const char *filename, const char *content)
{
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if (fd == -1)
    {
        perror("Failed to create test file");
        exit(1);
    }
    write(fd, content, strlen(content));
    close(fd);
}

// ============================================================================
// Test Functions
// ============================================================================

int test_my_open()
{
    // Setup
    const char *filename = "test_open_file.txt";
    create_test_file(filename, "hello");

    // Test case 1: Open for read ('r')
    int fd_r = my_open(filename, 'r');
    assert(fd_r != -1);
    close(fd_r);

    // Test case 2: Open for write ('w')
    // Note: The current implementation of my_open doesn't create files.
    // So we must ensure the file exists.
    int fd_w = my_open(filename, 'w');
    assert(fd_w != -1);
    close(fd_w);

    // Test case 3: Open for read/write ('o')
    int fd_o = my_open(filename, 'o');
    assert(fd_o != -1);
    close(fd_o);

    // Test case 4: Invalid mode
    int fd_invalid = my_open(filename, 'x');
    assert(fd_invalid == -1);

    // Test case 5: Open non-existent file for reading
    int fd_nonexistent = my_open("non_existent_file.txt", 'r');
    assert(fd_nonexistent == -1);

    // Teardown
    remove(filename);

    return 1; // Return 1 for success
}

int test_my_read_full()
{
    // Setup
    const char *filename = "test_read_file.txt";
    const char *content = "Hello, World!";
    create_test_file(filename, content);
    int fd = my_open(filename, 'r');
    assert(fd != -1);

    MyString str = new_string("");

    // Test case 1: Read a portion of the file
    int result = my_read_full(&str, fd, 5);
    assert(result == 0);
    MyString expected1 = new_string("Hello");
    assert(compare_strings(&str, &expected1) == 0);
    destroy_string(&expected1);

    // Test case 2: Read the full file (by reading more than exists)
    lseek(fd, 0, SEEK_SET); // Reset file pointer to the beginning
    result = my_read_full(&str, fd, 100);
    assert(result == 0);
    MyString expected2 = new_string("Hello, World!");
    assert(compare_strings(&str, &expected2) == 0);
    destroy_string(&expected2);

    // Test case 3: Read from an invalid file descriptor
    result = my_read_full(&str, -1, 10);
    assert(result == -1);

    // Teardown
    destroy_string(&str);
    my_close(fd);
    remove(filename);

    return 1;
}

int test_my_write_file()
{
    // Setup
    const char *filename = "test_write_file.txt";
    create_test_file(filename, "This will be overwritten.");

    int fd = my_open(filename, 'o'); // Open for read/write
    assert(fd != -1);

    MyString str_to_write = new_string("New Content");

    // Execute: write to the file (should overwrite)
    int bytes_written = my_write_file(&str_to_write, fd);
    assert(bytes_written == str_to_write.size);

    // Assert: Read back the content to verify
    MyString str_read = new_string("");
    lseek(fd, 0, SEEK_SET); // Go to start to read
    my_read_full(&str_read, fd, 100);

    assert(compare_strings(&str_read, &str_to_write) == 0);

    // Teardown
    destroy_string(&str_to_write);
    destroy_string(&str_read);
    my_close(fd);
    remove(filename);

    return 1;
}

int test_my_write_append()
{
    // Setup
    const char *filename = "test_append_file.txt";
    create_test_file(filename, "Initial.");

    int fd = my_open(filename, 'o');
    assert(fd != -1);

    MyString str_to_append = new_string(" Appended.");

    // Execute
    int bytes_written = my_write_append(&str_to_append, fd);
    assert(bytes_written == str_to_append.size);

    // Assert: Read back the full content to verify
    MyString str_read = new_string("");
    lseek(fd, 0, SEEK_SET);
    my_read_full(&str_read, fd, 100);

    MyString expected = new_string("Initial. Appended.");
    assert(compare_strings(&str_read, &expected) == 0);

    // Teardown
    destroy_string(&str_to_append);
    destroy_string(&str_read);
    destroy_string(&expected);
    my_close(fd);
    remove(filename);

    return 1;
}

int test_my_close()
{
    // Setup
    const char *filename = "test_close_file.txt";
    create_test_file(filename, "");
    int fd = my_open(filename, 'r');
    assert(fd != -1);

    // Test case 1: Close a valid file descriptor
    int result = my_close(fd);
    assert(result == 0);

    // Test case 2: Closing an already closed descriptor should fail
    result = my_close(fd);
    assert(result == -1);

    // Test case 3: Closing an invalid descriptor should fail
    result = my_close(-99);
    assert(result == -1);

    // Teardown
    remove(filename);

    return 1;
}

int main()
{
    printf("--- Starting IO Function Tests ---\n");

    RUN_TEST(test_my_open);
    RUN_TEST(test_my_read_full);
    RUN_TEST(test_my_write_file);
    RUN_TEST(test_my_write_append);
    RUN_TEST(test_my_close);

    printf("--- Test Summary ---\n");
    printf("Passed: %d, Failed: %d\n", tests_passed, tests_failed);

    return (tests_failed > 0); // Return 0 on success, 1 on failure
}
