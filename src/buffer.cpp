#include "buffer.h"
#include <assert.h>
#include <stdlib.h>

void BufferConstruct(Buffer *buff)
{
    assert(buff != nullptr);
    
    buff->count = 1;
    buff->curr = 0;
    
    buff->buffers = (char **)calloc(buff->count, sizeof(Buffer));
    
    for (int i = 0; i < buff->count; i++)
        buff->buffers[i] = (char*)calloc(DEFAULT_CAPACITY + 1, sizeof(char));
    
    buff->free = buff->buffers[0];    
}

void BufferResize(Buffer *buff)
{
    assert(buff != nullptr);
    
    buff->buffers = (char **)realloc(buff->buffers, sizeof(Buffer) * buff->count * 2);
    
    for (int i = buff->count; i < buff->count * 2; i++)
        buff->buffers[i] = (char *)calloc(DEFAULT_CAPACITY, sizeof(char));
    
    buff->count *= 2;
}

void BufferDecostruct(Buffer *buff)
{
    assert(buff != nullptr);
        
    for(int i = 0; i < buff->count; i++)
    {
        if (buff->buffers[i] != nullptr)
            free(buff->buffers[i]);
    }
    free(buff->buffers);
    
    buff->count = 0;
    buff->free = nullptr;
    buff->curr = -1;
}
