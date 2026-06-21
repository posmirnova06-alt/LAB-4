#pragma once
#include <stdexcept>
#include "MutableSequence.h"
#include "DynamicArray.h"

template <class T>
class MutableArraySequence : public MutableSequence<T>
{
private:
    DynamicArray<T> data;

public:
    MutableArraySequence() : data(0) {}

    MutableArraySequence(T* items, int count) : data(items, count) {}

    MutableArraySequence(const MutableArraySequence<T>& other) : data(other.data) {}

    T GetFirst() const override
    {
        if (data.GetSize() == 0)
        {
            throw std::out_of_range("Empty sequence");
        }

        return data.Get(0);
    }

    T GetLast() const override
    {
        if (data.GetSize() == 0)
        {
            throw std::out_of_range("Empty sequence");
        }

        return data.Get(data.GetSize()- 1);
    }

    T Get(int index) const override
    {
        return data.Get(index);
    }

    int GetLength() const override
    {
        return data.GetSize();
    }

    void Set(int index, const T& value) override
    {
        data.Set(index, value);
    }

    void Append(const T& item) override
    {
        data.Append(item);
    }

    void Prepend(const T& item) override
    {
        data.Prepend(item);
    }

    void InsertAt(const T& item, int index) override
    {
        data.InsertAt(item, index);
    }

    void RemoveAt(int index) override
    {
        data.RemoveAt(index);
    }

protected:
    MutableSequence<T>* CreateEmpty() const override
    {
        return new MutableArraySequence<T>();
    }
};