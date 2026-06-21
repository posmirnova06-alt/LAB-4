#pragma once
#include <stdexcept>
#include "MutableArraySequence.h"
#include "Generator.h"
#include "OrdinalIndex.h"

template <class T1, class T2>
class Pair
{
public:
    T1 first;
    T2 second;

    Pair() : first(T1()), second(T2()) {}
    Pair(T1 a, T2 b) : first(a), second(b) {}
};

template <class T>
class LazySequence
{
private:
    MutableArraySequence<T> cache;
    Generator<T>* generator;
    bool isInfinite;
    bool ownsGenerator;

    void materializeUpTo(int index)
    {
        while (cache.GetLength() <= index)
        {
            if (generator == nullptr)
            {
                throw std::out_of_range("Index out of range");
            }

            T next = generator->GetNext(cache);
            cache.Append(next);
        }
    }

public:
    LazySequence() : generator(nullptr), isInfinite(false), ownsGenerator(false) {}

    LazySequence(T* items, int count) : cache(items, count), generator(nullptr), isInfinite(false), ownsGenerator(false) {}

    LazySequence(const Sequence<T>* seq) : generator(nullptr), isInfinite(false), ownsGenerator(false)
    {
        for (int i = 0; i < seq->GetLength(); i++)
        {
            cache.Append(seq->Get(i));
        }
    }

    LazySequence(const LazySequence<T>& other) : cache(other.cache), generator(nullptr), isInfinite(other.isInfinite), ownsGenerator(false) {}

    LazySequence(T (*rule)(const MutableArraySequence<T>&), T* seedItems, int seedCount) : cache(seedItems, seedCount), isInfinite(true), ownsGenerator(true)
    {
        generator = new FunctionGenerator<T>(rule);
    }

    LazySequence(Generator<T>* gen, bool infinite) : generator(gen), isInfinite(infinite), ownsGenerator(true) {}

    ~LazySequence()
    {
        if (ownsGenerator && generator != nullptr)
        {
            delete generator;
        }
    }

    T Get(int index)
    {
        if (index < 0)
        {
            throw std::out_of_range("Negative index");
        }

        materializeUpTo(index);
        return cache.Get(index);
    }

    T Get(OrdinalIndex index)
    {
        if (index.isFinite())
        {
            return Get(index.finite);
        }

        if (generator == nullptr)
        {
            throw std::logic_error("No generator to handle transfinite index");
        }

        return generator->GetAt(index, cache);
    }

    T GetFirst()
    {
        return Get(0);
    }

    T GetLast()
    {
        if (isInfinite)
        {
            throw std::logic_error("Cannot get last of infinite sequence");
        }

        return cache.GetLast();
    }

    int GetMaterializedCount() const
    {
        return cache.GetLength();
    }

    bool IsInfinite() const
    {
        return isInfinite;
    }

    void Append(T item)
    {
        if (isInfinite)
        {
            throw std::logic_error("Cannot append to infinite sequence");
        }

        cache.Append(item);
    }

    void Prepend(T item)
    {
        if (isInfinite)
        {
            throw std::logic_error("Cannot prepend to infinite sequence");
        }

        cache.Prepend(item);
    }

    void InsertAt(T item, int index)
    {
        if (index < 0)
        {
            throw std::out_of_range("Negative index");
        }

        materializeUpTo(index);
        cache.InsertAt(item, index);
    }

    LazySequence<T>* GetSubsequence(int startIndex, int endIndex)
    {
        if (startIndex < 0 || endIndex < startIndex)
        {
            throw std::out_of_range("Invalid subsequence range");
        }

        LazySequence<T>* result = new LazySequence<T>();
        for (int i = startIndex; i <= endIndex; i++)
        {
            result->Append(Get(i));
        }

        return result;
    }

    LazySequence<T>* Where(bool (*predicate)(T))
    {
        WhereGenerator<T>* gen = new WhereGenerator<T>(this, predicate);
        return new LazySequence<T>(gen, isInfinite);
    }

    LazySequence<T>* Concat(LazySequence<T>* other)
    {
        OrdinalIndex fLen(0, 0);

        if (isInfinite)
        {
            fLen = OMEGA;
        }
        else
        {
            fLen = OrdinalIndex(0, GetMaterializedCountFull());
        }

        ConcatGenerator<T>* gen = new ConcatGenerator<T>(this, other, fLen);

        bool resultIsInfinite = false;

        if (isInfinite || other->IsInfinite())
        {
            resultIsInfinite = true;
        }

        return new LazySequence<T>(gen, resultIsInfinite);
    }

    LazySequence<Pair<T, T>>* Zip(LazySequence<T>* other)
    {
        ZipGenerator<T, T>* gen = new ZipGenerator<T, T>(this, other);
        bool resultIsInfinite = isInfinite && other->IsInfinite();
        return new LazySequence<Pair<T, T>>(gen, resultIsInfinite);
    }

    template <class T2>
    LazySequence<T2>* Map(T2 (*func)(T))
    {
        MapGenerator<T, T2>* gen = new MapGenerator<T, T2>(this, func);
        return new LazySequence<T2>(gen, isInfinite);
    }

    template <class T2>
    T2 Reduce(T2 (*func)(T2, T), T2 startValue, int limit)
    {
        T2 accumulator = startValue;
        for (int i = 0; i < limit; i++)
        {
            accumulator = func(accumulator, Get(i));
        }

        return accumulator;
    }

    int GetMaterializedCountFull()
    {
        int i = cache.GetLength();
        while (true)
        {
            try
            {
                Get(i);
                i++;
            }
            catch (const std::out_of_range&)
            {
                break;
            }
        }

        return i;
    }
};