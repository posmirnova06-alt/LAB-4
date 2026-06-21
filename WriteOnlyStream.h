#pragma once
#include <stdexcept>
#include "MutableArraySequence.h"

template <class T>
class WriteOnlyStream
{
private:
    MutableArraySequence<T>* destination;
    size_t position;
    bool isOpen;

public:
    WriteOnlyStream(MutableArraySequence<T>* dest) : destination(dest), position(0), isOpen(false) {}

    void Open()
    {
        isOpen = true;
    }

    void Close()
    {
        isOpen = false;
    }

    size_t Write(T item)
    {
        if (!isOpen)
        {
            throw std::logic_error("Stream is not open");
        }

        destination->Append(item);
        position++;
        return position;
    }

    size_t GetPosition() const
    {
        return position;
    }
};