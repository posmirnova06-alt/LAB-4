#pragma once
#include <stdexcept>
#include "MinHeap.h"

template <class T>
class MaxComparable
{
public:
    T value;

    MaxComparable() : value(T()) {}
    MaxComparable(T v) : value(v) {}

    bool operator<(const MaxComparable<T>& other) const
    {
        return value > other.value;
    }
};

template <class T>
class StatCollector
{
private:
    MinHeap<MaxComparable<T>> leftHeap;
    MinHeap<T> rightHeap;

    long long countTotal;
    T sum;
    T minValue;
    T maxValue;

    void Rebalance()
    {
        if (leftHeap.GetSize() > rightHeap.GetSize() + 1)
        {
            MaxComparable<T> moved = leftHeap.ExtractMin();
            rightHeap.Insert(moved.value);
        }
        else if (rightHeap.GetSize() > leftHeap.GetSize() + 1)
        {
            T moved = rightHeap.ExtractMin();
            leftHeap.Insert(MaxComparable<T>(moved));
        }
    }

public:
    StatCollector() : countTotal(0), sum(T()), minValue(T()), maxValue(T()) {}

    void AddValue(T x)
    {
        if (countTotal == 0)
        {
            minValue = x;
            maxValue = x;
        }
        else
        {
            if (x < minValue)
            {
                minValue = x;
            }

            if (x > maxValue)
            {
                maxValue = x;
            }
        }

        sum += x;
        countTotal++;

        if (leftHeap.IsEmpty() || x <= leftHeap.Top().value)
        {
            leftHeap.Insert(MaxComparable<T>(x));
        }
        else
        {
            rightHeap.Insert(x);
        }

        Rebalance();
    }

    long long GetCount() const
    {
        return countTotal;
    }

    double GetMean() const
    {
        if (countTotal == 0)
        {
            throw std::logic_error("No data collected");
        }

        return (double)sum / countTotal;
    }

    T GetMin() const
    {
        if (countTotal == 0)
        {
            throw std::logic_error("No data collected");
        }

        return minValue;
    }

    T GetMax() const
    {
        if (countTotal == 0)
        {
            throw std::logic_error("No data collected");
        }

        return maxValue;
    }

    double GetMedian() const
    {
        if (countTotal == 0)
        {
            throw std::logic_error("No data collected");
        }

        if (leftHeap.GetSize() == rightHeap.GetSize())
        {
            return (leftHeap.Top().value + rightHeap.Top()) / 2.0;
        }
        else if (leftHeap.GetSize() > rightHeap.GetSize())
        {
            return leftHeap.Top().value;
        }
        else
        {
            return rightHeap.Top();
        }
    }
};