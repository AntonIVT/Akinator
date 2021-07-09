#pragma once

/* Default capacity for 1 buffer */
const int DEFAULT_CAPACITY = 1000;

struct Buffer
{
    char **buffers;
    int count;
    int curr;
    char *free;
};

void BufferConstruct(Buffer *buff);

void BufferResize(Buffer *buff);

void BufferDecostruct(Buffer *buff);
