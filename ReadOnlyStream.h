#pragma once
#include <stdexcept>
#include "LazySequence.h"

template <class T>
class ReadOnlyStream
{
private:
    LazySequence<T>* source;
    size_t position;
    bool isOpen;

public:
    ReadOnlyStream(LazySequence<T>* seq) : source(seq), position(0), isOpen(false) {}

    void Open()
    {
        isOpen = true;
    }

    void Close()
    {
        isOpen = false;
    }

    bool IsEndOfStream() const
    {
        if (source->IsInfinite())
        {
            return false;
        }

        return position >= (size_t)source->GetMaterializedCount();
    }

    T Read()
    {
        if (!isOpen)
        {
            throw std::logic_error("Stream is not open");
        }

        if (IsEndOfStream())
        {
            throw std::out_of_range("End of stream reached");
        }

        T value = source->Get((int)position);
        position++;
        return value;
    }

    size_t GetPosition() const
    {
        return position;
    }

    bool IsCanSeek() const
    {
        return true;
    }

    size_t Seek(size_t index)
    {
        position = index;
        return position;
    }
};