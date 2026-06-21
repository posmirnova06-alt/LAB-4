#pragma once
#include "MutableSequence.h"
#include "LinkedList.h"

template <class T>
class MutableListSequence : public MutableSequence<T>
{
private:
    LinkedList<T> data;

public:
    MutableListSequence() : data() {}

    MutableListSequence(T* items, int count) : data(items, count) {}

    MutableListSequence(const MutableListSequence<T>& other) : data(other.data) {}

    T GetFirst() const override
    {
        if (data.GetLength() == 0)
        {
            throw std::out_of_range("Empty sequence");
        }
        return data.Get(0);
    }

    T GetLast() const override
    {
        if (data.GetLength() == 0)
        {
            throw std::out_of_range("Empty sequence");
        }
        return data.Get(data.GetLength() - 1);
    }

    T Get(int index) const override
    {
        return data.Get(index);
    }

    int GetLength() const override
    {
        return data.GetLength();
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
        return new MutableListSequence<T>();
    }
};