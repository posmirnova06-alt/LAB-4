#pragma once
#include "MutableArraySequence.h"
#include "OrdinalIndex.h"

template <class T>
class LazySequence;

template <class T1, class T2>
class Pair;

template <class T>
class Generator
{
public:
    virtual T GetNext(const MutableArraySequence<T>& cache) = 0;

    virtual T GetAt(OrdinalIndex index, MutableArraySequence<T>& cache)
    {
        if (!index.isFinite())
        {
            throw std::logic_error("This generator does not support transfinite indices");
        }

        int target = index.finite;
        while (cache.GetLength() <= target)
        {
            T next = GetNext(cache);
            cache.Append(next);
        }

        return cache.Get(target);
    }

    virtual bool HasNext() const
    {
        return true;
    }

    virtual bool TryGetNext(const MutableArraySequence<T>& cache, T& outValue)
    {
        if (!HasNext())
        {
            return false;
        }

        outValue = GetNext(cache);
        return true;
    }

    virtual ~Generator() = default;
};

template <class T>
class FunctionGenerator : public Generator<T>
{
private:
    T (*rule)(const MutableArraySequence<T>&);

public:
    FunctionGenerator(T (*r)(const MutableArraySequence<T>&)) : rule(r) {}

    T GetNext(const MutableArraySequence<T>& cache) override
    {
        return rule(cache);
    }
};

template <class T>
class WhereGenerator : public Generator<T>
{
private:
    LazySequence<T>* source;
    bool (*predicate)(T);
    int sourceIndex;

public:
    WhereGenerator(LazySequence<T>* src, bool (*pred)(T)) : source(src), predicate(pred), sourceIndex(0) {}

    T GetNext(const MutableArraySequence<T>& cache) override
    {
        while (true)
        {
            T candidate = source->Get(sourceIndex);
            sourceIndex++;
            if (predicate(candidate))
            {
                return candidate;
            }
        }
    }
};

template <class T, class T2>
class MapGenerator : public Generator<T2>
{
private:
    LazySequence<T>* source;
    T2 (*func)(T);
    int sourceIndex;

public:
    MapGenerator(LazySequence<T>* src, T2 (*f)(T)) : source(src), func(f), sourceIndex(0) {}

    T2 GetNext(const MutableArraySequence<T2>& cache) override
    {
        T value = source->Get(sourceIndex);
        sourceIndex++;
        return func(value);
    }
};

template <class T>
class ConcatGenerator : public Generator<T>
{
private:
    LazySequence<T>* first;
    LazySequence<T>* second;
    OrdinalIndex firstLength;
    OrdinalIndex current;

public:
    ConcatGenerator(LazySequence<T>* f, LazySequence<T>* s, OrdinalIndex fLen) : first(f), second(s), firstLength(fLen), current(0) {}

    T GetNext(const MutableArraySequence<T>& cache) override
    {
        OrdinalIndex pos = current;
        current = current + 1;

        if (pos < firstLength)
        {
            return first->Get(pos);
        }

        OrdinalIndex offsetInSecond = pos - firstLength;
        return second->Get(offsetInSecond.finite);
    }

    T GetAt(OrdinalIndex index, MutableArraySequence<T>& cache) override
    {
        if (index < firstLength)
        {
            return first->Get(index.finite);
        }

        OrdinalIndex offsetInSecond = index - firstLength;
        return second->Get(offsetInSecond.finite);
    }
};

template <class T1, class T2>
class ZipGenerator : public Generator<Pair<T1, T2>>
{
private:
    LazySequence<T1>* first;
    LazySequence<T2>* second;
    int index;

public:
    ZipGenerator(LazySequence<T1>* f, LazySequence<T2>* s) : first(f), second(s), index(0) {}

    Pair<T1, T2> GetNext(const MutableArraySequence<Pair<T1, T2>>& cache) override
    {
        T1 a = first->Get(index);
        T2 b = second->Get(index);
        index++;
        return Pair<T1, T2>(a, b);
    }
};