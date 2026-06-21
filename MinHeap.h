#pragma once
#include <stdexcept>
#include "DynamicArray.h"

template <class T>
class MinHeap
{
private:
    DynamicArray<T> data;
    int count;

    int Parent(int i) const
    {
        return (i - 1) / 2;
    }

    int Left(int i) const
    {
        return 2 * i + 1;
    }

    int Right(int i) const
    {
        return 2 * i + 2;
    }

    void SiftUp(int i)
    {
        while (i > 0 && data.Get(i) < data.Get(Parent(i)))
        {
            T temp = data.Get(i);
            data.Set(i, data.Get(Parent(i)));
            data.Set(Parent(i), temp);
            i = Parent(i);
        }
    }

    void SiftDown(int i)
    {
        while (true)
        {
            int left = Left(i);
            int right = Right(i);
            int smallest = i;

            if (left < count && data.Get(left) < data.Get(smallest))
            {
                smallest = left;
            }

            if (right < count && data.Get(right) < data.Get(smallest))
            {
                smallest = right;
            }

            if (smallest == i)
            {
                break;
            }

            T temp = data.Get(i);
            data.Set(i, data.Get(smallest));
            data.Set(smallest, temp);

            i = smallest;
        }
    }

public:
    MinHeap() : data(0), count(0) {}

    void Insert(const T& value)
    {
        if (count == data.GetSize())
        {
            if (count == 0)
            {
                data.Resize(1);
            }
            else
            {
                data.Resize(count * 2);
            }
        }

        data.Set(count, value);
        count++;
        SiftUp(count - 1);
    }

    T Top() const
    {
        if (count == 0)
        {
            throw std::out_of_range("Heap is empty");
        }

        return data.Get(0);
    }

    T ExtractMin()
    {
        if (count == 0)
        {
            throw std::out_of_range("Heap is empty");
        }

        T result = data.Get(0);

        count--;
        data.Set(0, data.Get(count));
        SiftDown(0);

        return result;
    }

    int GetSize() const
    {
        return count;
    }

    bool IsEmpty() const
    {
        return count == 0;
    }
};