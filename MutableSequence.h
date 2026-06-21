#pragma once
#include "Sequence.h"

template <class T>
class MutableSequence : public Sequence<T>
{
public:
    virtual ~MutableSequence() = default;
    
    virtual void Append(const T& item) = 0;
    virtual void Prepend(const T& item) = 0;
    virtual void InsertAt(const T& item, int index) = 0;
    virtual void RemoveAt(int index) = 0;
    virtual void Set(int index, const T& value) = 0;

    MutableSequence<T>* Map(T (*func)(const T&)) const
    {
        MutableSequence<T>* result = CreateEmpty();
        for (int i = 0; i < this->GetLength(); i++)
        {
            result->Append(func(this->Get(i)));
        }
        return result;
    }

    MutableSequence<T>* Where(bool (*predicate)(const T&)) const
    {
        MutableSequence<T>* result = CreateEmpty();
        for (int i = 0; i < this->GetLength(); i++)
        {
            if (predicate(this->Get(i)))
            {
                result->Append(this->Get(i));
            }
        }
        return result;
    }

    T Reduce(T (*func)(const T&, const T&), T initial) const
    {
        T result = initial;
        for (int i = 0; i < this->GetLength(); i++)
        {
            result = func(result, this->Get(i));
        }
        return result;
    }

    MutableSequence<T>* Concat(const Sequence<T>& other) const
    {
        MutableSequence<T>* result = CreateEmpty();
        for (int i = 0; i < this->GetLength(); i++)
        {    
            result->Append(this->Get(i));
        }
        for (int i = 0; i < other.GetLength(); i++)
        {
            result->Append(other.Get(i));
        }
        return result;
    }

    MutableSequence<T>* GetSubsequence(int start, int end) const
    {
        if (start < 0 || end < 0 || start >= this->GetLength() || end >= this->GetLength() || start > end)
        {
            throw std::out_of_range("Invalid range");
        }

        MutableSequence<T>* result = CreateEmpty();
        for (int i = start; i <= end; i++)
        {    
            result->Append(this->Get(i));
        }
        return result;
    }

protected:
    virtual MutableSequence<T>* CreateEmpty() const = 0;
};