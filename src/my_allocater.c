#include "my_allocater.h"
#include <unistd.h>
/*
 * Some standards :
 * NULL is the error code
 * ff : First Fit
 */

static MemoryChunk *free_list_head = NULL;

void my_initalize_allocated_mem_metadata(MemoryChunk *new_chunk, size_t size)
{
    new_chunk->is_free = FALSE;
    new_chunk->size = size;
    new_chunk->next_chunk = NULL;
    new_chunk->prev_chunk = NULL;
}
void my_append_freed_mem(MemoryChunk *new_chunk)
{
    if (new_chunk == NULL)
        return;

    new_chunk->next_chunk = NULL;
    new_chunk->prev_chunk = NULL;

    if (free_list_head == NULL)
    {
        free_list_head = new_chunk;
        return;
    }

    MemoryChunk *current = free_list_head;
    while (current->next_chunk != NULL)
    {
        current = current->next_chunk;
    }

    current->next_chunk = new_chunk;
    new_chunk->prev_chunk = current;
    new_chunk->is_free = TRUE;
}

MemoryChunk *my_search_for_chunk_ff(size_t requested_size)
{
    MemoryChunk *current = free_list_head;
    while (current != NULL)
    {
        if (current->size >= requested_size)
            return current;
        current = current->next_chunk;
    }
    return NULL;
}

void my_remove_chunk(MemoryChunk *chunk_to_remove)
{
    if (chunk_to_remove == NULL)
        return;

    if (chunk_to_remove == free_list_head)
    {
        free_list_head = chunk_to_remove->next_chunk;
        chunk_to_remove->next_chunk = NULL;
        return;
    }

    if (chunk_to_remove->prev_chunk != NULL)
    {
        chunk_to_remove->prev_chunk->next_chunk = chunk_to_remove->next_chunk;
        if (chunk_to_remove->next_chunk != NULL)
        {
            chunk_to_remove->next_chunk->prev_chunk = chunk_to_remove->prev_chunk;
        }
        chunk_to_remove->next_chunk = NULL;
        chunk_to_remove->prev_chunk = NULL;
    }
    chunk_to_remove->is_free = FALSE;
}
void *my_os_request(size_t requested_size)
{
    size_t header_size = sizeof(MemoryChunk);
    size_t total_size = requested_size + header_size;

    void *raw_memory = sbrk(total_size);
    if (raw_memory == (void *)-1)
    {
        return NULL;
    }

    MemoryChunk *chunk = (MemoryChunk *)raw_memory;
    my_initalize_allocated_mem_metadata(chunk, requested_size);
    return (void *)(chunk + 1); // pointer to memory after the header
}

void my_split_chunk(MemoryChunk *chunk_ptr, size_t requested_size)
{
    size_t remaining_size = chunk_ptr->size - sizeof(MemoryChunk) - requested_size;

    if (remaining_size < sizeof(MemoryChunk))
    {
        return;
    }

    MemoryChunk *new_block = (MemoryChunk *)((char *)(chunk_ptr + 1) + requested_size);
    // Holly arithethic

    new_block->size = remaining_size;
    new_block->next_chunk = chunk_ptr->next_chunk;
    new_block->prev_chunk = chunk_ptr;

    if (chunk_ptr->next_chunk)
        chunk_ptr->next_chunk->prev_chunk = new_block;
    // Holly linked lists shit

    chunk_ptr->next_chunk = new_block;
    chunk_ptr->size = requested_size + sizeof(MemoryChunk);
    new_block->is_free = TRUE; // can it be even forgot
}

void *my_allocate(size_t requested_size)
{
    MemoryChunk *free_chunk = my_search_for_chunk_ff(requested_size);
    if (free_chunk == NULL)
    {
        return my_os_request(requested_size);
    }
    else
    {
        my_split_chunk(free_chunk, requested_size);
        my_remove_chunk(free_chunk);
        return (void *)(free_chunk + 1);
    }
}
void my_free(void *pointer)
{
    // arithmethic Holliness ahead
    //
    MemoryChunk *chunk = (MemoryChunk *)((char *)(pointer) - sizeof(MemoryChunk));
    my_append_freed_mem(chunk);
}
