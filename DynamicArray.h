#pragma once 
#include <stdexcept>

template <class T> 
class DynamicArray 
{
private:
    T* data;
    int size;
    int capacity;

public:
    DynamicArray(int size)
    {
        if (size < 0)
        {
            throw std::out_of_range("Negative size");
        }

        this->size = size;
        this->capacity = size;

        if (size == 0)
        {
            data = nullptr;
        }
        else
        {
            data = new T[size];
        }
    }

    DynamicArray(T* items, int count)
    {
        if (count < 0)
        {
            throw std::out_of_range("Negative size");
        }

        size = count;
        capacity = count;

        if (size == 0)
        {
            data = nullptr;
            return;
        }

        data = new T[size];

        for (int i = 0; i < size; i++)
        {
            data[i] = items[i];
        }
    }

    DynamicArray(const DynamicArray<T>& other)
    {
        size = other.size;
        capacity = other.size;

        if (size == 0)
        {
            data = nullptr;
            return;
        }

        data = new T[size];

        for (int i = 0; i < size; i++)
        {
            data[i] = other.data[i];
        }
    }

    ~DynamicArray()
    {
        delete[] data;
    }

    T Get(int index) const
    {
        if (index < 0 || index >= size)
        {
            throw std::out_of_range("Index out of range");
        }

        return data[index];
    }

    void Set(int index, const T& value)
    {
        if (index < 0 || index >= size)
        {
            throw std::out_of_range("Index out of range");
        }

        data[index] = value;
    }

    int GetSize() const
    {
        return size;
    }

    void Resize(int newSize)
    {
        if (newSize < 0)
        {
            throw std::out_of_range("Negative size");
        }

        T* newData = nullptr;

        if (newSize > 0)
        {
            newData = new T[newSize];

            int minSize;
            if (size < newSize)
            {
                minSize = size;
            }
            else
            {
                minSize = newSize;
            }

            for (int i = 0; i < minSize; i++)
            {
                newData[i] = data[i];
            }
        }

        delete[] data;

        data = newData;
        size = newSize;
        capacity = newSize;
    }

    void Append(const T& value)
    {
        if (size == capacity)
        {
            int newCapacity;
            if (capacity == 0)
            {
                newCapacity = 1;
            }
            else
            {
                newCapacity = capacity * 2;
            }

            T* newData = new T[newCapacity];

            for (int i = 0; i < size; i++)
            {
                newData[i] = data[i];
            }

            delete[] data;
            data = newData;
            capacity = newCapacity;
        }

        data[size] = value;
        size++;
    }

    void Prepend(const T& value)
    {
        T* newData = new T[size + 1];

        newData[0] = value;

        for (int i = 0; i < size; i++)
        {
            newData[i + 1] = data[i];
        }

        delete[] data;

        data = newData;
        size++;
        capacity = size;
    }

    void InsertAt(const T& value, int index)
    {
        if (index < 0 || index > size)
        {
            throw std::out_of_range("Index out of range");
        }

        T* newData = new T[size + 1];

        for (int i = 0; i < index; i++)
        {
            newData[i] = data[i];
        }

        newData[index] = value;
        
        for (int i = index; i < size; i++)
        {
            newData[i + 1] = data[i];
        }

        delete[] data;

        data = newData;
        size++;
        capacity = size;
    }

    void RemoveAt(int index)
    {
        if (size == 0)
        {
            throw std::out_of_range("Array is empty");
        }

        if (index < 0 || index >= size)
        {
            throw std::out_of_range("Index out of range");
        }

        T* newData = new T[size - 1];

        for (int i = 0; i < index; i++)
        {
            newData[i] = data[i];
        }

        for (int i = index + 1; i < size; i++)
        {
            newData[i - 1] = data[i];
        }

        delete[] data;

        data = newData;
        size--;
        capacity = size;
    }
};