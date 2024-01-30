#pragma once

#include <iostream>
#include <memory>
#include <memory.h>

class ByteCircularBuffer {
private:
    unsigned char *buffer;
    int size;
    int front;
    int rear;
    int count;
public:
    ByteCircularBuffer(int size);
    ~ByteCircularBuffer();
    int write(char* data, int size);
    char* read(int size);
    bool isEmpty();
    bool isFull();
    int availableSize();
    void clear();
};