#include "../../include/ByteCircularBuffer.hpp"

ByteCircularBuffer::ByteCircularBuffer(int size) {
    this->buffer = new unsigned char[size];
    this->size = size;
    this->front = 0;
    this->rear = 0;
    this->count = 0;
}

ByteCircularBuffer::~ByteCircularBuffer() {
    delete[] this->buffer;
}

bool ByteCircularBuffer::isEmpty() {
    return this->count == 0;
}

bool ByteCircularBuffer::isFull() {
    return this->count == this->size;
}

int ByteCircularBuffer::availableSize() {
    return this->size - this->count;
}

int ByteCircularBuffer::write(char* data, int data_size) {
    if (this->isFull()) {
        std::cout << "Buffer is full" << std::endl;
        return 0;
    }

    if (data_size > availableSize()) {
        std::cout << "Data size is bigger than available size" << std::endl;
        return 0;
    }

    if (this->rear + data_size > this->size) {
        int first_size = this->size - this->rear;
        int second_size = data_size - first_size;
        memcpy(this->buffer + this->rear, data, first_size);
        memcpy(this->buffer, data + first_size, second_size);
        this->count += data_size;
        this->rear = second_size;

        std::cout << buffer << std::endl;
        return data_size;
    }

    memcpy(this->buffer + this->rear, data, data_size);
    this->count += data_size;
    this->rear = (this->rear + data_size) % this->size;

    std::cout << buffer << std::endl;

    return data_size;
}

char* ByteCircularBuffer::read(int data_size) {
    if (this->isEmpty()) {
        std::cout << "Buffer is empty" << std::endl;
        return nullptr;
    }

    if (data_size > this->count) {
        std::cout << "Data size is bigger than available size" << std::endl;
        return nullptr;
    }

    if (this->front + data_size > this->size) {
        int first_size = this->size - this->front;
        int second_size = data_size - first_size;
        char *data = new char[data_size];
        memcpy(data, this->buffer + this->front, first_size);
        memcpy(data + first_size, this->buffer, second_size);
        this->count -= data_size;
        this->front = second_size;

        return data;
    }

    char *data = new char[data_size];
    memcpy(data, this->buffer + this->front, data_size);
    this->count -= data_size;
    this->front = (this->front + data_size) % this->size;

    return data;
}

void ByteCircularBuffer::clear() {
    this->front = 0;
    this->rear = 0;
    this->count = 0;
}